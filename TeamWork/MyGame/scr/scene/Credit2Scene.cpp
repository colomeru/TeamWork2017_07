#include "Credit2Scene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"
#include "../fade/FadePanel.h"

#include "../actor/player/Player3.h"
#include "../actor/enemy/Enemy.h"
#include "../graphic/Model.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../actor/Field/Clothes/TestClothes.h"
#include "../actor/Field/Clothes/Hanger/Hanger.h"
#include "../graphic/Sprite.h"
#include "../actor/Field/Clothes/Hairball/HairballGenerator/HairballGenerator.h"
#include "../game/Random.h"
#include "GamePlayDefine.h"
#include"../actor/Field/ClothesLine.h"
#include"../tween/TweenManager.h"
#include "addScreen\screenSupport\CreditTextGenerator.h"
#include "../time/Time.h"
#include "../Def.h"
#include "../actor/player/CreditPlayer.h"
#include "../scene/addScreen/screenSupport/CreditText.h"
#include "../actor/DummyActor.h"
#include "../sound/sound.h"

Credit2Scene::Credit2Scene() :
	nextScene_(Scene::Title)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	bgScreen_ = BackgroundScreen(world_.get());
	textScreen_ = TutorialTextScreen(world_.get());
}

Credit2Scene::~Credit2Scene()
{
	
}

void Credit2Scene::Initialize()
{

	isEnd_ = false;
	isRetry_ = false;

	world_->Initialize();

	player_ = std::make_shared<CreditPlayer>(world_.get());
	//credit_ = std::make_shared<CreditText>(world_.get(),CLOTHES_ID::TEXT_CLOTHES, 1, Vector2(0.0f, 0.0f),0);

	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	//stageGeneratorManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Tutorial"), 0));
	//stageGeneratorManager.SetStage(Stage::Stage1);
	std::string filename = "Tutorial1";
	Stage1 stage(world_.get(), filename);
	//stage.CreateClothes();
	//setWindTime(Stage::Stage1);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));

	world_->SetTarget(player_.get());
	//world_->SetTarget(credit_.get());


	bgScreen_.Init(Stage::Stage1);
	textScreen_.Init("Tutorial.txt");

	//world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 0, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(WINDOW_WIDTH + 100, 0), Vector2(-150, 100)));
	//world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 2, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));

	//
	correction = Vector2(300.0f, 500.0f);
	wCorr = Vector2(-40, -20);

	startPos_ = Vector2(WINDOW_WIDTH / 2.0f - correction.x, -110.0f);

	pHeadPos_ = startPos_;
	//player_->setCurPHeadSPos(playerPos_);
	//player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(playerPos_.x, playerPos_.y, 0)));
	playerStatte_ = RESTART;
	for (int i = 0; i < 6; i++) {
		spritePos_[i] = Vector2(200.0f, WINDOW_HEIGHT + 200.0f + i * 600.0f);
	}
	spriteSize_ = Vector2(800.0f, 200.0f);
	operate_ = true;
	sceneTimer_ = 0.0f;
	test = false;
	test2 = false;
	test3 = true;
	alpha_ = 0.0f;
	sinCount_ = 0;

	//カラス
	whitePos_ = { player_->GetCurrentPHeadPosition() + correction + wCorr,
				Vector2(WINDOW_WIDTH / 2.0f, -300.0f) + wCorr };
	dWhitePos_ = whitePos_[0];
	waiting_ = true;
	anmManager_.Add(SPRITE_ID::WHITE_ANM_01_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_02_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_03_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_04_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_05_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_06_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_07_SPRITE);
	anmManager_.Add(SPRITE_ID::WHITE_ANM_08_SPRITE);
	anmManager_.SetIsRepeat(true);

	Sound::GetInstance().PlayBGM(BGM_ID::CREDIT_BGM, DX_PLAYTYPE_LOOP);

	//world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<CreditText>(world_.get(), Vector2(500, 500)));
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<CreditTextGenerator>(world_.get(), Vector2(500, 100)));


	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

void Credit2Scene::Update()
{
	// 更新
	world_->Update();

	world_->SetScrollPos(Vector2::Zero);

	bgScreen_.Update();
	//textScreen_.Update();

	anmManager_.Update();

	if (!FadePanel::GetInstance().IsClearScreen()) return;

	//風テスト
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H)) {
		world_->sendMessage(EventMessage::BEGIN_WIND);
	}

	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();


	// 終了
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM8))
	//{
	//	FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
	//	FadePanel::GetInstance().FadeOut();
	//}

	//if (isRetry_) {
	//	End();
	//	Initialize();
	//}

	whitePos_[0] = player_->GetCurrentPHeadPosition() + correction + wCorr;

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		pHeadPos_.x += 5.0f;
		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));

	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		pHeadPos_.x -= 5.0f;
		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));

	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		pHeadPos_.y -= 5.0f;
		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));

	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN)) {
		pHeadPos_.y += 5.0f;
		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));
	}

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::L)) {
		player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
	}

	if (!player_->GetIsBiteMode()) playerStatte_ = FALL;

	PlayerRestart();

	Scroll();




	sceneTimer_ += Time::DeltaTime;
	if (sceneTimer_ >= SceneTime) {
		sinCount_ += 5; sinCount_ %= 360;
		alpha_ = MathHelper::Sin(sinCount_);
		test2 = true;
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}
}

void Credit2Scene::Draw() const
{
	bgScreen_.Draw();

	//for (int i = 0; i < 6; i++) {
	//	auto min = spritePos_[i] - Vector2(spriteSize_.x / 2.0f, spriteSize_.y / 2.0f);
	//	auto max = spritePos_[i] + Vector2(spriteSize_.x / 2.0f, spriteSize_.y / 2.0f);
	//	DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255 - 20 * i, 20 * i), 1);
	//}


	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);


	//textScreen_.Draw();

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	//Vector2 wPos;
	//if (operate_) wPos = whitePos_[1];
	//else wPos = whitePos_[0];
	anmManager_.Draw(dWhitePos_, origin, Vector2::One, 1.0f);

	//if (sceneTimer_ >= SceneTime) {
	//	Vector2 ssorigin = Sprite::GetInstance().GetSize(SPRITE_ID::BACKTITLE_TEXT_SPRITE) / 2;
	//	Sprite::GetInstance().Draw(SPRITE_ID::BACKTITLE_TEXT_SPRITE, Vector2(WINDOW_WIDTH / 2, 750.0f), ssorigin, alpha_, Vector2::One);
	//}
	if (BuildMode != 1) return;
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "PHeadyPos_:%f %f", pHeadPos_.x, pHeadPos_.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "PlayerPos_:%f %f", player_->GetCurrentPHeadPosition().x, player_->GetCurrentPHeadPosition().y);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "spritePos_:%f %f", spritePos_[1].x, spritePos_[1].y);

	if (test) DrawFormatString(0, 100, GetColor(255, 255, 255), "触れている");
	else DrawFormatString(0, 100, GetColor(255, 255, 255), "触れていない");

	if (operate_) DrawFormatString(0, 120, GetColor(255, 255, 255), "操作できます！");
	else DrawFormatString(0, 120, GetColor(255, 255, 255), "操作できません！");

	DrawFormatString(0, 140, GetColor(255, 255, 255), "sceneTimer_:[%f]", sceneTimer_);

	if (test2) DrawFormatString(0, 160, GetColor(255, 255, 255), "遷移できます！");
	else DrawFormatString(0, 160, GetColor(255, 255, 255), "遷移できません！");

	DrawFormatString(0, 180, GetColor(255, 255, 255), "PlayerPos_:%f %f", player_->GetPosition().x, player_->GetPosition().y);



	DrawLine(0, 500, WINDOW_WIDTH, 500, GetColor(255, 0, 0), 1);
	DrawLine(300, 0, 300, WINDOW_HEIGHT, GetColor(255, 0, 0), 1);
	DrawLine(0, player_->GetCurrentPHeadPosition().y, WINDOW_WIDTH, player_->GetCurrentPHeadPosition().y, GetColor(0, 255, 0), 1);
	DrawLine(player_->GetCurrentPHeadPosition().x, 0, player_->GetCurrentPHeadPosition().x, WINDOW_HEIGHT, GetColor(0, 255, 0), 1);
	DrawLine(0, WINDOW_HEIGHT / 2.0f, WINDOW_WIDTH, WINDOW_HEIGHT / 2.0f, GetColor(0, 0, 255), 1);
	DrawLine(WINDOW_WIDTH / 2.0f, 0, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT, GetColor(0, 0, 255), 1);
	DrawLine(0, player_->GetCurrentPHeadPosition().y + correction.y, WINDOW_WIDTH, player_->GetCurrentPHeadPosition().y + correction.y, GetColor(0, 0, 0), 1);
	DrawLine(player_->GetCurrentPHeadPosition().x + correction.x, 0, player_->GetCurrentPHeadPosition().x + correction.x, WINDOW_HEIGHT, GetColor(0, 0, 0), 1);


}

bool Credit2Scene::IsEnd() const
{
	return isEnd_;
}

Scene Credit2Scene::Next() const
{
	return nextScene_;
}

void Credit2Scene::End()
{
	// 初期化
	world_->Clear();
	bgScreen_.End();

	Sound::GetInstance().StopBGM();

	//FadePanel::GetInstance().AddCollBack([=] {FadePanel::GetInstance().FadeIn(); });
	//FadePanel::GetInstance().FadeOut();
}

void Credit2Scene::handleMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
		break;
	case EventMessage::STRONG_WIND:
		break;
	case EventMessage::ATTENUATE_WIND:
		break;
	case EventMessage::END_WIND:
		break;
	case EventMessage::START_LANE_CHANGE:
		break;
	case EventMessage::GOAL_FLAG:
		break;
	case EventMessage::GAME_CLEAR_FLAG:
		break;
	case EventMessage::TAPPER_DEAD:
		break;
	case EventMessage::PLAY_NEXT_STAGE:
		break;
	case EventMessage::ADD_SCORE:
		break;
	case EventMessage::PLAYER_DEAD:
	{
		//isRetry_ = true;
		FadePanel::GetInstance().AddCollBack([=]() { Initialize(); });
		FadePanel::GetInstance().FadeOut();
		break;
	}
	default:
		break;
	}
}

//画面外か？
bool Credit2Scene::ScreenOut() const
{
	//if (player_->GetCurrentPHeadPosition().x - correction.x < -300.0f || player_->GetCurrentPHeadPosition().x - correction.x > WINDOW_WIDTH + 300.0f ||
	//	player_->GetCurrentPHeadPosition().y - correction.y < -300.0f || player_->GetCurrentPHeadPosition().y - correction.y > WINDOW_HEIGHT + 300.0f)
	if (player_->GetCurrentPHeadPosition().x < -300.0f - correction.x || player_->GetCurrentPHeadPosition().x > WINDOW_WIDTH + 300.0f - correction.x ||
		player_->GetCurrentPHeadPosition().y < -300.0f - correction.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 300.0f - correction.y)
		//if (player_->GetCurrentPHeadPosition().x < -250.0f - correction.x || player_->GetCurrentPHeadPosition().x > WINDOW_WIDTH + 250.0f - correction.x ||
		//	player_->GetCurrentPHeadPosition().y < -250.0f - correction.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 250.0f - correction.y - 300.0f)
		return true;
	else
		return false;
}

//プレイヤーリスタート
void Credit2Scene::PlayerRestart()
{
	if (!operate_) {
		player_->SetIsBiteMode(true);

		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));

		dWhitePos_ = whitePos_[0];

		if (test3) {
			player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
			test3 = false;
		}
	}

	//画面外に出たら
	if ((ScreenOut() || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::K)) &&
		operate_) {
		operate_ = false;
		player_->AllResurrectHead();
		//player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
		player_->SetIsBiteMode(false);
		player_->CurHeadBite(pHeadPos_);
		player_->PHeadLengthReset();
		playerStatte_ = RESTART;
		pHeadPos_ = player_->GetCurrentPHeadPosition(); //プレイヤー座標
		//player_->GravityReset();
		//player_->CurHeadBite(pHeadPos_);

		player_->SetOtherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES);
		TweenManager::GetInstance().Add(EaseOutQuart, &pHeadPos_, startPos_, 2.0f);
		//auto dummy = std::make_shared<DummyActor>(world_.get());
		//world_->Add(ACTOR_ID::DUMMY_ACTOR, dummy);
		//world_->PushStackActor(dummy);
		int flag = 0;
		world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);

		Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
	}

	//スタート位置に戻ったら
	Vector2 dis = player_->GetCurrentPHeadPosition() - startPos_;
	if (dis.Length() <= 2.0f && !operate_) {

		//player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
		player_->SetIsBiteMode(true);
		player_->GravityReset();
		operate_ = true;
		waiting_ = true;
		test3 = true;

		//world_->PopStackActor();
		//world_->EachActor(ACTOR_ID::DUMMY_ACTOR, [](Actor& other) {other.Dead(); });
		int flag = 1;
		world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);

	}

	if (waiting_ && (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2))) {
		TweenManager::GetInstance().Add(EaseInQuart, &dWhitePos_, whitePos_[1], 2.0f);
		waiting_ = false;
	}

}

//スクロール
void Credit2Scene::Scroll()
{
	for (int i = 0; i < 6; i++) {
		spritePos_[i].y -= 2.0f;
	}
}

//画像に触れているか？
bool Credit2Scene::IsCollision()
{
	for (int i = 0; i < 6; i++) {
		if (player_->GetCurrentPHeadPosition().x > spritePos_[i].x - spriteSize_.x / 2.0f &&
			player_->GetCurrentPHeadPosition().x < spritePos_[i].x + spriteSize_.x / 2.0f &&
			player_->GetCurrentPHeadPosition().y > spritePos_[i].y - spriteSize_.y / 2.0f &&
			player_->GetCurrentPHeadPosition().y < spritePos_[i].y + spriteSize_.y / 2.0f) {
			return true;
		}
		else return false;
	}



}

bool Credit2Scene::GetOperate()
{
	return  operate_;
}
