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

Credit2Scene::Credit2Scene() :
	nextScene_(Scene::Menu)
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
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(WINDOW_WIDTH + 100, 0), Vector2(-150, 0)));
	//world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 2, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));

	//
	correction = Vector2(300.0f, 500.0f);

	startPos_ = Vector2(WINDOW_WIDTH / 2.0f - correction.x, 0.0f);

	pHeadPos_ = startPos_;
	//player_->setCurPHeadSPos(playerPos_);
	//player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(playerPos_.x, playerPos_.y, 0)));
	playerStatte_ = RESTART;
	for (int i = 0; i < 6; i++) {
		spritePos_[i] = Vector2(200.0f, WINDOW_HEIGHT + 200.0f + i * 600.0f);
	}
	spriteSize_ = Vector2(800.0f, 200.0f);
	operate_ = true;

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

	if (!FadePanel::GetInstance().IsClearScreen()) return;

	//風テスト
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H)) {
		world_->sendMessage(EventMessage::BEGIN_WIND);
	}

	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();


	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM8))
	{
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

	//if (isRetry_) {
	//	End();
	//	Initialize();
	//}


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

	if (!player_->GetIsBiteMode()) playerStatte_ = FALL;

	PlayerRestart();

	Scroll();

	//if ((Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M) ||
	//	GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2)) &&
	//	IsCollision()) {
	//	playerStatte_ = BITE;
	//	player_->SetIsBiteMode(true);
	//}

	if (IsCollision()) test = true;
	else test = false;

}

void Credit2Scene::Draw() const
{
	bgScreen_.Draw();

	//for (int i = 0; i < 6; i++) {
	//	auto min = spritePos_[i] - Vector2(spriteSize_.x / 2.0f, spriteSize_.y / 2.0f);
	//	auto max = spritePos_[i] + Vector2(spriteSize_.x / 2.0f, spriteSize_.y / 2.0f);
	//	DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255 - 20 * i, 20 * i), 1);
	//}

	Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, Vector2(-150, 350));

	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);


	//textScreen_.Draw();

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

	TweenManager::GetInstance().Clear();

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
		player_->GetCurrentPHeadPosition().y < -300.0f - correction.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 300.0f - correction.y - 300.0f)
		return true;
	else
		return false;
}

//プレイヤーリスタート
void Credit2Scene::PlayerRestart()
{
	//画面外に出たら
	if ((ScreenOut() || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::K)) &&
		operate_) {
		operate_ = false;

		player_->PHeadLengthReset();
		player_->AllResurrectHead();
		player_->SetIsBiteMode(true);
		player_->PHeadLengthReset();
		playerStatte_ = RESTART;
		pHeadPos_ = player_->GetCurrentPHeadPosition(); //プレイヤー座標
		player_->GravityReset();
		player_->CurHeadBite(pHeadPos_);
		//player_->SetIsBiteMode(true);
		player_->SetOtherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES);
		TweenManager::GetInstance().Add(EaseOutExpo, &pHeadPos_, startPos_, 2.0f);
	}

	//スタート位置に戻ったら
	if (((pHeadPos_.x >= startPos_.x - 2.0f && pHeadPos_.x <= startPos_.x + 2.0f) &&
		(pHeadPos_.y >= startPos_.y - 2.0f && pHeadPos_.y <= startPos_.y + 2.0f)) &&
		!operate_) {
		operate_ = true;
	}

	if (!operate_) {
		player_->setCurPHeadSPos(pHeadPos_);
		player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));
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
