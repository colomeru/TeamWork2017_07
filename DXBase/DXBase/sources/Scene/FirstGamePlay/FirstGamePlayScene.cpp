#include "FirstGamePlayScene.h"
#include "../../World/World.h"
#include "../../Field/Field.h"
#include "../../Actor/Base/ActorGroup.h"
#include "../../Actor/Person/Player/Player.h"
//#include "../../Actor/Person/Enemy/Enemy.h"
//#include "../../Actor/Person/Enemy/EnemySpawner.h"
#include "../../ResourceLoader/ResourceLoader.h"

#include "../../Actor/Person/Enemy/Enemys/FloorTurnEnemy.h"
#include "../../Actor/Person/Enemy/Enemys/WallTurnEnemy.h"
#include "../../Field/MapGenerator.h"
#include "../../Actor/BackGraundManager/BackGraundManager.h"
#include <memory>
#include <random>


FirstGamePlayScene::FirstGamePlayScene(SceneDataKeeper* keeper) :nextScene_(Scene::GameOver), isStopped_(false) {
	isEnd_ = false;
	keeper_ = keeper;
	//name_はcsvファイル名と合わせる事
	name_ = "stage00";
	deltaTime_ = 1 / 60.f;

	//値の表示を行う
	numberTexes_.push_back(TextureID::NUMBER_ZERO_TEX);
	numberTexes_.push_back(TextureID::NUMBER_ONE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_TWO_TEX);
	numberTexes_.push_back(TextureID::NUMBER_THREE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_FOUR_TEX);
	numberTexes_.push_back(TextureID::NUMBER_FIVE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_SIX_TEX);
	numberTexes_.push_back(TextureID::NUMBER_SEVEN_TEX);
	numberTexes_.push_back(TextureID::NUMBER_EIGHT_TEX);
	numberTexes_.push_back(TextureID::NUMBER_NINE_TEX);
}

FirstGamePlayScene::~FirstGamePlayScene() {
}

void FirstGamePlayScene::start() {
	//クリア時の表示用クラス
	clear_ = ClearScreen(keeper_);
	isClearStage_ = false;

	deltaTime_ = Time::GetInstance().deltaTime();
	stageTime_ = 0.0f;

	//Pauseかどうかを判別する
	isStopped_ = false;

	stageAlpha_ = 0.0f;
	stageTime_ = 0.0f;
	stageFlag_ = true;

	//SetDrawScreen(DX_SCREEN_BACK);
	world_ = std::make_shared<World>(keeper_);
	world_->CollisitionOffOn(true);
	MapGenerator gener = MapGenerator(world_.get());

	//csvのファイル名とnameを合わせる事
	world_->SetPlayerPos(gener.findStartPoint("./resources/file/" + name_ + ".csv"));

	//world_->addEventMessageListener(
	//	[=](EventMessage msg, void* param) {
	//	handleMessage(msg, param);
	//}
	//);
	//world_->addField(std::make_shared<Field>("./resources/file/" + name_ + ".csv", 0, 0, stg));
	world_->addActor(ActorGroup::Player, std::make_shared<Player>(world_.get(), gener.findStartPoint("./resources/file/" + name_ + ".csv")));
	//world_->addActor(ActorGroup::Enemy, std::make_shared<FloorTurnEnemy>(world_.get(), START_POS + Vector2(200, -200)));
	//world_->addActor(ActorGroup::Enemy, std::make_shared<WallTrunEnemy>(world_.get(), Vector2(250, 325)));

	gener.create("./resources/file/" + name_ + ".csv", 0, 0,1);

	//scenedatakeeperにこのステージの名前を渡す
	keeper_->setCurrentSceneName(name_);

	Vector2 csvSize = gener.GetCellSize();// Vector2(gener.GetColumnSize(), gener.GetRowSize());
	world_->SetScreenPlayerPos(SCREEN_SIZE / 2);
	world_->SetScroolJudge(Vector2(1, 1), world_->GetScreenPlayerPos(), Vector2(csvSize.x*CHIPSIZE - (SCREEN_SIZE.x - world_->GetScreenPlayerPos().x), csvSize.y*CHIPSIZE - (SCREEN_SIZE.y - world_->GetScreenPlayerPos().y)));

	backManager = new BackGraundManager(world_.get());
	float graundPos = -(world_->GetScreenPlayerPos().y)*1.5f;
	backManager->BossFlag(false);

	backManager->SetBackGraund(TextureID::BACKSTAGE1_2_TEX, TextureID::BACKSTAGE1_2_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_3_TEX, TextureID::BACKSTAGE1_3_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_4_TEX, TextureID::BACKSTAGE1_4_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_5_TEX, TextureID::BACKSTAGE1_5_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_6_1_TEX, TextureID::BACKSTAGE1_6_1_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_6_1_TEX, TextureID::BACKSTAGE1_6_2_TEX, graundPos, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_7_TEX, TextureID::BACKSTAGE1_7_TEX, graundPos*1.5f, false, false, Vector2(2, 2));
	backManager->SetBackGraund(TextureID::BACKSTAGE1_8_TEX, TextureID::BACKSTAGE1_8_TEX, graundPos*2.5f, true, false, Vector2(2, 2));
	backManager->AddKonoha(TextureID::HAPPA1_1_TEX);
	backManager->AddKonoha(TextureID::HAPPA1_2_TEX);
	backManager->SetUpBackGraund(TextureID::BACKSTAGE1_1_TEX, 8);
	world_->clear(false);

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123), DX_PLAYTYPE_LOOP);
	//playerFont_ = new PlayerFont(world_.get(), Vector2::Zero);

	//playerFont_->AddEvent(TxtEvent::START_EVENT, "./resources/file/Txt/Start.txt", 5);
	//playerFont_->AddEvent(TxtEvent::STARGET_EVENT, "./resources/file/Txt/StarGet.txt", 2);
	//playerFont_->AddEvent(TxtEvent::GOOL_EVENT, "./resources/file/Txt/Gool.txt", 4);

	//playerFont_->ChangeFont(TxtEvent::START_EVENT);
	//playerFont_->StartFont();

	world_->PlayerNotMove(true);
	eventFlag1_ = true;
	eventFlag2_ = true;

	tutorialMgr_.load_csv("./resources/file/stage00_come.csv");
	tutorialMgr_.init();
	keeper_->start();
	previousScoreKeeper_ = 0;
	currentScoreKeeper_ = 0;

	world_->currentSceneName_ = name_;
}

void FirstGamePlayScene::update() {
	//isClearStage_がtrueになったら、ステージクリア時のリザルト画面のみを更新する
	if (isClearStage_) {
		clear_.start(name_);
		if (clear_.update(name_, nextScene_)) {
			isEnd_ = true;
		};
		return;
	}
	//if (!playerFont_->GetEndFont()) world_->PlayerNotMove(false);
	//if (keeper_->GetItemCount() >= 1 && eventFlag1_) {
	//	eventFlag1_ = false;
	//	playerFont_->ChangeFont(TxtEvent::STARGET_EVENT);
	//	playerFont_->StartFont();
	//}
	//if (world_->is_clear() && eventFlag2_) {
	//	eventFlag2_ = false;
	//	playerFont_->ChangeFont(TxtEvent::GOOL_EVENT);
	//	playerFont_->StartFont();
	//}
	//if (!eventFlag2_&&!playerFont_->GetEndFont()) {
	//	isClearStage_ = world_->is_clear();
	//}

	keeper_->update(deltaTime_);

	if (stageFlag_) {
		stageAlpha_ += Time::GetInstance().deltaTime();
		if (stageAlpha_ >= 1.0f) {
			stageFlag_ = false;
		}
	}
	else
	{
		stageTime_ += Time::GetInstance().deltaTime();
		if (stageTime_ >= 3.0f) stageAlpha_ -= Time::GetInstance().deltaTime();
	}
	stageAlpha_ = MathHelper::Clamp(stageAlpha_, 0.0f, 1.0f);

	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_START)) {
		isStopped_ ? deltaTime_ = Time::GetInstance().deltaTime() : deltaTime_ = 0;
		isStopped_ = !isStopped_;
	}
	world_->update(deltaTime_);

	backManager->Update(deltaTime_);

	//ここからゲームが終了しているかの判別
	auto player = world_->findActor("Player");



	if (player == nullptr) {
		isEnd_ = true;
		nextScene_ = Scene::GameOver;
	}
	if (world_->is_clear()) {
		nextScene_ = Scene::StageClear;
	}


	if (!isEnd_) {
		isStopped_ ? isEnd_ = pause_.update(nextScene_) : isEnd_ = move_.update(name_, nextScene_, isClearStage_);
	}
	//keeper_->setItemCount(world_->getCount());
	//ここまでゲームが終了しているかの判別
	//playerFont_->Update();

	tutorialMgr_.update(world_.get(), Time::GetInstance().deltaTime());

	if (tutorialMgr_.isTutorialEnd()) {
		isClearStage_ = true;
	}
	previousScoreKeeper_ = currentScoreKeeper_;
	currentScoreKeeper_ = keeper_->GetItemCount();
}

void FirstGamePlayScene::draw() const {
	backManager->Draw();
	//world描画
	world_->draw();

	backManager->BackDraw();
	//playerFont_->Draw();
	tutorialMgr_.draw();

	//クリア時はClear専用のシーンを表示
	if (isClearStage_) {
		clear_.draw();
		return;
	}


	//ここからアイテム取得数の描画
	int drawNum = keeper_->GetItemCount();
	int baseNum = keeper_->GetItemCount();
	int posCount = 0;
	std::vector<int> drawNumberList;
	drawNumberList.clear();
	for (;;) {

		if (baseNum < 10) {
			drawNumberList.push_back(baseNum);
			break;
		}
		drawNum = (int)(baseNum*0.1);
		drawNum = drawNum * 10;
		int textNum = baseNum - drawNum;

		drawNumberList.push_back(textNum);
		baseNum = (int)(baseNum*0.1);
		posCount++;
	}
	float starScoreSize = 1;
	if (previousScoreKeeper_ < keeper_->GetItemCount()) {
		starScoreSize = 1.5f;
	}
	for (int i = 0; i < (int)drawNumberList.size(); i++) {
		DrawRotaGraph2((int)((SCREEN_SIZE.x)
			- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x*(i + 1)) + (ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x / 2), 50 + (ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).y / 2), ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x / 2, ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).y / 2, starScoreSize, 0, ResourceLoader::GetInstance().getTextureID(numberTexes_[drawNumberList[i]]), TRUE);
	}

	int ans = (int)log10(keeper_->GetItemCount()) + 1;
	if (keeper_->GetItemCount() == 0)ans = 1;

	DrawRotaGraph2((int)(SCREEN_SIZE.x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).x)*(ans + 1)) + (ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).x / 2), 50 + (ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).y / 2), (ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).x / 2), (ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).y / 2), starScoreSize, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);

	//ここまでアイテム数の描画
	if (keeper_->getComboLimit() > 0)drawCombo();

	isStopped_ ? pause_.draw() : move_.draw();
}

void FirstGamePlayScene::end() {
	delete backManager;
	//delete playerFont_;
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
}

bool FirstGamePlayScene::isEnd() const {

	return isEnd_;
}

Scene FirstGamePlayScene::next() const {
	return nextScene_;
}

void FirstGamePlayScene::drawCombo() const
{
	//int drawNumber(int targetNum, int posx, int posy, int NumTexType = 1, int maxSize = 3, float drawSize = 1.f) const {
	int targetNum = keeper_->getComboCount();
	int posx = SCREEN_SIZE.x - ((ResourceLoader::GetInstance().GetTextureSize(TextureID::COMBO_TEX).x)) - ((ResourceLoader::GetInstance().GetTextureSize(TextureID::NUMBER_ZERO_TEX).x));
	int posy = 200;
	int maxSize = 4;
	float drawSize = 1.f;
	if (keeper_->getComboLimit() >= 3.f)drawSize = 1.5f;

	int drawNum = targetNum;
	int baseNum = targetNum;
	int posCount = 0;
	std::vector<int> drawNumberList;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)MathHelper::Lerp(0.f, 255.f, keeper_->getComboLimit() / 3));
	for (int i = 0; i < maxSize; i++) {

		if (baseNum < 10) {
			drawNumberList.push_back(baseNum);
			break;
		}

		drawNum = (int)(baseNum*0.1);
		drawNum = drawNum * 10;
		int textNum = baseNum - drawNum;
		drawNumberList.push_back(textNum);
		baseNum = (int)(baseNum*0.1);
		posCount++;
	}

	int drawx = posx;// +ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[0]]).x*(maxSize - 1);

	for (int i = 0; i < maxSize; i++) {
		//drawNumberListの桁数がmaxSizeを下回っていた場合、桁数以上のloopをカットする
		if (i >= drawNumberList.size()) {
			continue;
		}
		DrawRotaGraph2(drawx - (ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x*i*drawSize), posy + 40, 0, 0, drawSize, 0, ResourceLoader::GetInstance().getTextureID(numberTexes_[drawNumberList[i]]), TRUE);
	}

	DrawGraph((int)(SCREEN_SIZE.x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::COMBO_TEX).x)), posy, ResourceLoader::GetInstance().getTextureID(TextureID::COMBO_TEX), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
