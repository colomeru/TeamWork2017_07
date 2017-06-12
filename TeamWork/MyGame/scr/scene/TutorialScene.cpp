
#include "TutorialScene.h"
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

TutorialScene::TutorialScene() :
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

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	isEnd_ = false;
	isRetry_=false;
	FadePanel::GetInstance().Initialize();

	world_->Initialize();

	player_ = std::make_shared<Player>(world_.get());
	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	//stageGeneratorManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Tutorial"), 0));
	//stageGeneratorManager.SetStage(Stage::Stage1);
	std::string filename="Tutorial";
	Stage1 stage(world_.get(), filename, 0);
	stage.CreateClothes();
	//setWindTime(Stage::Stage1);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));
	world_->SetTarget(player_.get());

	bgScreen_.Init(Stage::Stage1);
	textScreen_.Init("Tutorial.txt");

	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 0, stage.GetStageSize()+Vector2(150,0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize()+Vector2(150,0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 2, stage.GetStageSize()+Vector2(150,0), Vector2(0, 0)));
}

void TutorialScene::Update()
{
	// 更新
	world_->Update();

	//風テスト
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H)) {
		world_->sendMessage(EventMessage::BEGIN_WIND);
	}

	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();

	bgScreen_.Update();
	textScreen_.Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H)|| GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM8))
		isEnd_ = true;

	if (isRetry_) {
		End();
		Initialize();

	}
}

void TutorialScene::Draw() const
{
	bgScreen_.Draw();
	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	textScreen_.Draw();

	if (BuildMode != 1) return;
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);


}

bool TutorialScene::IsEnd() const
{
	return isEnd_;
}

Scene TutorialScene::Next() const
{
	return nextScene_;
}

void TutorialScene::End()
{
	// 初期化
	world_->Clear();
	bgScreen_.End();
}

void TutorialScene::handleMessage(EventMessage message, void * param)
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
		isRetry_ = true;
		break;
	default:
		break;
	}
}

