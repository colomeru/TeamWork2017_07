#include "StageClearScene.h"
#include "../../ResourceLoader/ResourceLoader.h"
#include "../../Actor/Person/Player/Player.h"
#include "../../Actor/BackGraundManager/BackGraundManager.h"
#include "../../Actor/Person/Player/PlayerBody.h"
#include "../../Actor/Person/Player/PlayerConnector.h"
#include "../../Actor/Base/ActorPtr.h"
#include "../../Actor/Base/ActorGroup.h"

static const Vector2 BaseBossPosition = Vector2(-250, 500);
static const int MaxRotate = 60;
static const int blockCount = 30;
static const int floorSpeed = 5;

StageClearScene::StageClearScene(SceneDataKeeper* keeper){
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "StageClear";
	nextScene = Scene::GamePlay;
	BoardTexes.push_back(TextureID::BOARD_STAGE2_TEX);
	BoardTexes.push_back(TextureID::BOARD_STAGE3_TEX);
	BoardTexes.push_back(TextureID::BOARD_STAGE4_TEX);
	BoardTexes.push_back(TextureID::BOARD_NULL_TEX);
	BoardTexes.push_back(TextureID::BOARD_NULL_TEX);

	BlockTexes.push_back(TextureID::FLOOR_STAGE1_TEX);
	BlockTexes.push_back(TextureID::FLOOR_STAGE2_TEX);
	BlockTexes.push_back(TextureID::FLOOR_STAGE3_TEX);
	BlockTexes.push_back(TextureID::FLOOR_STAGE4_TEX);
	BlockTexes.push_back(TextureID::FLOOR_STAGE1_TEX);
}

StageClearScene::~StageClearScene() {

}

void StageClearScene::start() {
	SetDrawScreen(DX_SCREEN_BACK);

	backManager = new BackGraundManager();
	backNum_ = 0;
	floorPosition_ = 0;

	isArrive_ = false;
	isFirstIdle = true;
	isFirstJump = true;

	BackTitles id;
	id.push_back(TextureID::BACKSTAGE1_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_2_TEX);
	id.push_back(TextureID::BACKSTAGE1_3_TEX);
	id.push_back(TextureID::BACKSTAGE1_4_TEX);
	id.push_back(TextureID::BACKSTAGE1_5_TEX);
	id.push_back(TextureID::BACKSTAGE1_6_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_6_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_7_TEX);
	id.push_back(TextureID::BACKSTAGE1_8_TEX);
	BackTitles id2;
	id2.push_back(TextureID::BACKSTAGE2_TOP1_TEX);
	id2.push_back(TextureID::BACKSTAGE2_1_TEX);
	id2.push_back(TextureID::BACKSTAGE2_2_TEX);
	id2.push_back(TextureID::BACKSTAGE2_3_TEX);
	id2.push_back(TextureID::BACKSTAGE2_4_TEX);
	id2.push_back(TextureID::BACKSTAGE2_TOP2_TEX);
	id2.push_back(TextureID::BACKSTAGE2_5_TEX);
	id2.push_back(TextureID::BACKSTAGE2_6_TEX);
	id2.push_back(TextureID::BACKSTAGE2_7_TEX);
	id2.push_back(TextureID::BACKSTAGE2_8_TEX);
	/*id2.push_back(TextureID::BACKSTAGE2_9_TEX);
	id2.push_back(TextureID::BACKSTAGE2_10_TEX);*/
	BackTitles id3;
	id3.push_back(TextureID::BACKSTAGE4_1_TEX);
	id3.push_back(TextureID::BACKSTAGE4_2_TEX);

	titleTexs.push_back(id);
	titleTexs.push_back(id2);
	titleTexs.push_back(id3);

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_INTER), DX_PLAYTYPE_LOOP);

	if (keeper_->getSceneName() == "stage01") {
		backNum_ = 0;
		boardNum_ = 0;
		blockNum_ = 0;
		//PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123), DX_PLAYTYPE_LOOP);
	}
	if (keeper_->getSceneName() == "stage02") {
		backNum_ = 0;
		boardNum_ = 1;
		blockNum_ = 1;
		//PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123), DX_PLAYTYPE_LOOP);
	}
	if (keeper_->getSceneName() == "stage03"){
		backNum_ = 1;
		boardNum_ = 2;
		blockNum_ = 2;
		//PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_4), DX_PLAYTYPE_LOOP);
	}
	if (keeper_->getSceneName() == "stage04") {
		backNum_ = 2;
		boardNum_ = 3;
		blockNum_ = 3;
	//	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_5), DX_PLAYTYPE_LOOP);
	}
	if (keeper_->getSceneName() == "stage00") {
		backNum_ = 0;
		boardNum_ = 4;
		blockNum_ = 4;
		//PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123), DX_PLAYTYPE_LOOP);
	}
	for (int i = 0; i < (int)titleTexs[backNum_].size(); i++) {
		backManager->SetBackGraund(titleTexs[backNum_][i], titleTexs[backNum_][i]);
	}


	//Worldを生成し、プレイヤーを追加
	world_ = std::make_shared<World>(keeper_);
	player_ = std::make_shared<Player>(world_.get(), Vector2(-400, 850));
	world_->addActor(ActorGroup::Player, player_);
	Vector2 clampPoint = Vector2(-400, 0);
	player_->setClampPoint(clampPoint);
	//world_->addActor(ActorGroup::Player, std::make_shared<Player>(world_.get(), Vector2(-400, 850)));

	//プレイヤーは自動での移動、判定等は行わないようにする
	world_->PlayerNotMove(true);
	world_->CollisitionOffOn(true);

	//経過時間の初期化
	mIvemtTime = 0;

	//ボスの初期位置を設定する
	bossPosition = BaseBossPosition;

	//アニメーションの初期化
	anmer_ = SceneChangeBossAnm();

	nextScene = Scene::GamePlay;

	//stage04をクリアした場合のみ、次のシーンをボスステージに変更する
	if (keeper_->getSceneName() == "stage04")nextScene = Scene::BossStage01;

	speedx = 0;
	speedy = 0;
	stopx = 0.5f;
	bossRotate = MaxRotate;

	BoardPosition = SCREEN_SIZE-Vector2(0,ResourceLoader::GetInstance().GetTextureSize(BoardTexes[boardNum_]).y);

	for (int i = -10; i < 30; i++) {
		world_->addActor(ActorGroup::Field, std::make_shared<MovelessFloor>(
			//ResourceLoader::GetInstance().getTextureID(TextureID::GOAL_TEX), world_.get(), Vector2(i*CHIPSIZE, 950)));
			ResourceLoader::GetInstance().getTextureID(TextureID::BOARD_NULL_TEX), world_.get(), Vector2(i*CHIPSIZE, 950)));
	}
	world_->currentSceneName_ = name_;
	
}
void StageClearScene::update() {

	if (InputMgr::GetInstance().IsPushButton()) {
		isEnd_ = true;
	}

	//アニメーションの更新
	anmer_.update_e(Time::GetInstance().deltaTime());

	if (!isArrive_) {
		backManager->Update(0, true);

		floorPosition_ += floorSpeed;
		if (floorPosition_ > CHIPSIZE) {
			int def = (int)(floorPosition_-CHIPSIZE);
			floorPosition_ = def;
		}
	}
	//経過時間の更新
	mIvemtTime += Time::GetInstance().deltaTime();
	//～1秒の間は何もしない
	if (mIvemtTime <= 1.0f) {
	}
	//～3秒の間はボスを右上に移動する
	else if (mIvemtTime <= 3.0f) {
		stopx+=Time::GetInstance().deltaTime()*10;
		
		stopx= min(max(stopx, 0), 1);

		speedx += 1.5f-stopx;
		//speedx = min(max(speedx,0),1);
		
		float lerpPos = speedx*0.05;
		lerpPos = min(max(lerpPos, 0), 1);
		//bossPosition += Vector2(800, -200)*Time::GetInstance().deltaTime();
		bossPosition = Vector2::Lerp(BaseBossPosition,Vector2(1400,100),lerpPos);
	
		if (isFirstIdle&&lerpPos>=1) {
			isFirstIdle = false;
			anmer_.setIdle();
			anmer_.Turn();
		}
		else if(lerpPos>=1){
			bossRotate -= 5;
			bossRotate = max(0, bossRotate);
		}
	}
	else if (mIvemtTime <= 5) {

		dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody1").get())->ForcedMove(Vector2(400.f, 0.0f));
		dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody2").get())->ForcedMove(Vector2(400.f, 0.0f));
		auto conn = world_->findActor("PlayerConnector").get();
		if (conn != nullptr) dynamic_cast<PlayerConnector*>(world_->findActor("PlayerConnector").get())->ForcedMove(Vector2(400.0f, 0.0f));
	}
	else if (mIvemtTime <= 7.f) {
	}
	else if (mIvemtTime <= 9.0f) {
		if (isFirstJump) {
			isFirstJump = false;
			anmer_.setJump();
			anmer_.Turn();
		}

		bossPosition += Vector2(1200, -300)*Time::GetInstance().deltaTime();

		bossRotate += 5;
		bossRotate = min(MaxRotate, bossRotate);

		BoardPosition.x-=5;

	}
	else if(mIvemtTime <= 12.0f) {
		dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody1").get())->ForcedMove(Vector2(800, 0.0f));
		dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody2").get())->ForcedMove(Vector2(800.0f, 0.0f));
		auto conn = world_->findActor("PlayerConnector").get();
		if (conn != nullptr) dynamic_cast<PlayerConnector*>(world_->findActor("PlayerConnector").get())->ForcedMove(Vector2(800.0f, 0.0f));

		isArrive_ = true;
	}
	//全ての工程が終了したら、シーンを終了する
	else {
		isEnd_ = true;
	}
	
	//World(プレイヤー)の更新を行う
	world_->update(Time::GetInstance().deltaTime());
}

void StageClearScene::draw() const {
	//World(プレイヤー)の描画
	backManager->Draw(true);
	
	for (int i = 0; i < blockCount; i++)
	{
		DrawGraph(i*CHIPSIZE - floorPosition_, 950, ResourceLoader::GetInstance().getTextureID(BlockTexes[blockNum_]), TRUE);
		DrawGraph(i*CHIPSIZE-floorPosition_, 950+CHIPSIZE, ResourceLoader::GetInstance().getTextureID(BlockTexes[blockNum_]), TRUE);
	}

	world_->draw();

	//ボスの描画
	anmer_.draw_e(bossPosition,Vector2::Zero,2,bossRotate);

	DrawGraph(BoardPosition.x, BoardPosition.y, ResourceLoader::GetInstance().getTextureID(BoardTexes[boardNum_]), TRUE);

}

void StageClearScene::end() {
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_INTER));
	//if (keeper_->getSceneName() == "stage01") {
	//	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
	//}
	//if (keeper_->getSceneName() == "stage02") {
	//	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
	//}
	//if (keeper_->getSceneName() == "stage03") {
	//	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_4));
	//}
	//if (keeper_->getSceneName() == "stage04") {
	//	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_5));
	//}
	//if (keeper_->getSceneName() == "stage00") {
	//	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
	//}
}

bool StageClearScene::isEnd() const {
	return isEnd_;
}

Scene StageClearScene::next() const {
	return nextScene;
}
