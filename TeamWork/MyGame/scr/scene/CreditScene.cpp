#include "CreditScene.h"
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
#include "../debugdata/DebugDraw.h"
#include "../actor/player/PlayerNeck/PlayerNeckPendulumSupport.h"

CreditScene::CreditScene() :
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

CreditScene::~CreditScene()
{

}

void CreditScene::Initialize()
{

	isEnd_ = false;
	isRetry_ = false;

	world_->Initialize();

	player_ = std::make_shared<CreditPlayer>(world_.get());

	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	std::string filename = "Tutorial1";
	Stage1 stage(world_.get(), filename);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));

	world_->SetTarget(player_.get());

	bgScreen_.Init(Stage::Stage1);
	textScreen_.Init("Tutorial.txt");

	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(WINDOW_WIDTH + 100, 0), Vector2(-150, 100)));

	//
	correction = Vector2(300.0f, 500.0f);
	wCorr = Vector2(-40, -20);

	startPos_ = Vector2(WINDOW_WIDTH / 2.0f - correction.x, -110.0f);

	pHeadPos_ = startPos_;
	operate_ = true;
	sceneTimer_ = 0.0f;
	sceneChange_ = false;
	mulInit_ = true;

	//カラス
	whitePos_ = { player_->GetCurrentPHeadPosition() + correction + wCorr,
		Vector2(WINDOW_WIDTH / 2.0f, -300.0f) + wCorr };
	dWhitePos_ = whitePos_[0];
	waiting_ = true;
	int crowIdNum = WHITE_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);

	Sound::GetInstance().PlayBGM(BGM_ID::CREDIT_BGM, DX_PLAYTYPE_LOOP);

	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<CreditTextGenerator>(world_.get(), Vector2(500, 100)));


	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

void CreditScene::Update()
{
	// 更新
	world_->Update();

	world_->SetScrollPos(Vector2::Zero);

	bgScreen_.Update();

	anmManager_.Update();

	if (!FadePanel::GetInstance().IsClearScreen()) return;

	whitePos_[0] = player_->GetCurrentPHeadPosition() + correction + wCorr;

	PlayerRestart();

	sceneTimer_ += Time::DeltaTime;
	if (sceneTimer_ >= SceneTime) {
		sceneChange_ = true;
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

}

void CreditScene::Draw() const
{
	bgScreen_.Draw();

	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(dWhitePos_, origin, Vector2::One, 1.0f);

	if (BuildMode != 1) return;
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "PHeadyPos_:%f %f", pHeadPos_.x, pHeadPos_.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "PlayerPos_:%f %f", player_->GetCurrentPHeadPosition().x, player_->GetCurrentPHeadPosition().y);

	if (operate_) DrawFormatString(0, 120, GetColor(255, 255, 255), "操作できます！");
	else DrawFormatString(0, 120, GetColor(255, 255, 255), "操作できません！");

	DrawFormatString(0, 140, GetColor(255, 255, 255), "sceneTimer_:[%f]", sceneTimer_);

	if (sceneChange_) DrawFormatString(0, 160, GetColor(255, 255, 255), "遷移できます！");
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

bool CreditScene::IsEnd() const
{
	return isEnd_;
}

Scene CreditScene::Next() const
{
	return nextScene_;
}

void CreditScene::End()
{
	// 初期化
	world_->Clear();
	bgScreen_.End();

	Sound::GetInstance().StopBGM();
}

void CreditScene::handleMessage(EventMessage message, void * param)
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
		FadePanel::GetInstance().AddCollBack([=]() { Initialize(); });
		FadePanel::GetInstance().FadeOut();
		break;
	}
	default:
		break;
	}
}

//画面外か？
bool CreditScene::ScreenOut() const
{
	if (player_->GetCurrentPHeadPosition().x < -300.0f - correction.x || player_->GetCurrentPHeadPosition().x > WINDOW_WIDTH + 300.0f - correction.x ||
		player_->GetCurrentPHeadPosition().y < -300.0f - correction.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 300.0f - correction.y)
		return true;
	else
		return false;
}

//プレイヤーリスタート
void CreditScene::PlayerRestart()
{
	if (!operate_) {
		player_->SetIsBiteMode(true);

		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));

		dWhitePos_ = whitePos_[0];

		if (mulInit_) {
			player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
			mulInit_ = false;
		}
	}

	//画面外に出たら
	if ((ScreenOut() || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::K)) &&
		operate_) {
		operate_ = false;
		player_->AllResurrectHead();
		player_->SetIsBiteMode(false);
		player_->CurHeadBite(pHeadPos_);
		player_->PHeadLengthReset();
		pHeadPos_ = player_->GetCurrentPHeadPosition(); //プレイヤー座標
		player_->SetOtherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES);
		TweenManager::GetInstance().Add(EaseOutQuart, &pHeadPos_, startPos_, 2.0f);
		int flag = 0;
		world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);
		Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
		//TweenManager::GetInstance().Add(EaseOutQuart, &dWhitePos_, whitePos_[0], 0.5f);
	}

	//スタート位置に戻ったら
	Vector2 dis = player_->GetCurrentPHeadPosition() - startPos_;
	if (dis.Length() <= 2.0f && !operate_) {
		player_->SetIsBiteMode(true);
		player_->GravityReset();
		operate_ = true;
		waiting_ = true;
		mulInit_ = true;
		int flag = 1;
		world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);
	}

	if (waiting_ && (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2))) {
		TweenManager::GetInstance().Add(EaseInQuart, &dWhitePos_, whitePos_[1], 2.0f);
		waiting_ = false;
	}
}
