#include "CreditScene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"

#include "../actor/player/Player.h"
#include "../actor/stage/Stage.h"
#include "../graphic/Model.h"

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

	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();

	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<Stage>(world_.get()));
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<Player>(world_.get()));
}

void CreditScene::Update()
{
	// 更新
	world_->Update();

	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void CreditScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// 描画
	world_->Draw();
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
