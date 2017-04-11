#include "TitleScene.h"
#include "../Def.h"
#include "../time/Time.h"

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
	isEnd_ = false;
}

void TitleScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void TitleScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "TitleScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

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
