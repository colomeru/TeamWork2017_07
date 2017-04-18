#include "TitleScene.h"
#include "../Def.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../actor/PlayerLine.h"

TitleScene::TitleScene() :
nextScene_(Scene::Menu)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(Vector3(0, 0, -10));
	Camera::GetInstance().Target.Set(Vector3(0, 0, 0));
	Camera::GetInstance().Update();
	isEnd_ = false;

	world_->Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<PlayerLine>(world_.get()));
}

void TitleScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
	Camera::GetInstance().Position.Set(Vector3 (0,0,-50));
	Camera::GetInstance().Target.Set(Vector3(0,0,0));
	Camera::GetInstance().Update();
}

void TitleScene::Draw() const
{
	//auto pos1 = DXConverter::GetInstance().ToVECTOR(Vector3(0, 0, 0));
	//auto pos2 = DXConverter::GetInstance().ToVECTOR(Vector3(0, 0, 0));
	//DrawFormatString(0, 00, GetColor(255, 255, 255), "TitleScene");
	//DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	//DrawCapsule3D(pos1, pos2, 4.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), FALSE);
	// 描画
	world_->Draw();
}

bool TitleScene::IsEnd() const
{
	return isEnd_;
}

Scene TitleScene::Next() const
{
	return nextScene_;
}

void TitleScene::End()
{
	// 初期化
	world_->Clear();
}

void TitleScene::handleMessage(EventMessage message, void * param)
{
}
