#include "MenuScene.h"
#include "../time/Time.h"
#include "../math/MathHelper.h"
#include "../graphic//Sprite.h"

MenuScene::MenuScene() :
	nextScene_(Scene::GamePlay)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
	isEnd_ = false;
}

void MenuScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;

	menu.Update();
}

void MenuScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "MenuScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	menu.Draw();

	// 描画
	world_->Draw();
}

bool MenuScene::IsEnd() const
{
	return isEnd_;
}

Scene MenuScene::Next() const
{
	return nextScene_;
}

void MenuScene::End()
{
	// 初期化
	world_->Clear();
}

void MenuScene::handleMessage(EventMessage message, void * param)
{
}
