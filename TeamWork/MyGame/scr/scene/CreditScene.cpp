#include "CreditScene.h"
#include "../time/Time.h"

CreditScene::CreditScene() :
nextScene_(Scene::Ending)
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
}

void CreditScene::Update()
{
	// 更新
	world_->Update();

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
