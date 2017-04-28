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

CreditScene::CreditScene() :
nextScene_(Scene::Ending),
camera_pos_(0, 100, -100),
target_(0, 0, 0)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

CreditScene::~CreditScene()
{
}

void CreditScene::Initialize()
{
	isEnd_ = false;

	//Camera::GetInstance().SetRange(0.1f, 10000.0f);
	//Camera::GetInstance().SetViewAngle(60.0f);
	//Camera::GetInstance().Up.Set(Vector3::Up);
	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();

	FadePanel::GetInstance().Initialize();


	player_ = std::make_shared<Player>(world_.get());
	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);

	stageGeneratorManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Stage1")));
	stageGeneratorManager.SetStage(Stage::Stage1);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));
	world_->SetTarget(player_.get());

	size = stageGeneratorManager.GetStageSize(Stage::Stage1);
}

void CreditScene::Update()
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

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void CreditScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);


	DrawFormatString(0, 200, GetColor(255, 255, 255), "stageSize x:%f y:%f", size.x, size.y);
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
}

void CreditScene::handleMessage(EventMessage message, void * param)
{
}

