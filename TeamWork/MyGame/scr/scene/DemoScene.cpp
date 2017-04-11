#include "DemoScene.h"
#include "../fade/FadePanel.h"

void DemoScene::Initialize()
{
	isEnd_ = false;

	// フェードイン
	FadePanel::GetInstance().FadeIn();
}

void DemoScene::Update()
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void DemoScene::Draw() const
{
	DrawFormatString(0,  0, GetColor(255, 255, 255), "DemoScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "Spaceで次のシーン");
}

bool DemoScene::IsEnd() const
{
	return isEnd_;
}

Scene DemoScene::Next() const
{
	return Scene::Title;
}

void DemoScene::End()
{
}
