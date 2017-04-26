#include "DemoScene.h"
#include "../fade/FadePanel.h"

DemoScene::DemoScene() :
maxLoadContentCount_(static_cast<float>(GetASyncLoadNum())),
currentLoadContentCount_(0.0f)
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::Initialize()
{
	isEnd_ = false;

	// フェードイン
	FadePanel::GetInstance().FadeIn();
}

void DemoScene::Update()
{
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
	//	isEnd_ = true;

	if (GetASyncLoadNum() == 0 && ProcessMessage() == 0)
	{
		FadePanel::GetInstance().SetOutTime(0.0f);
		FadePanel::GetInstance().FadeOut();
		isEnd_ = true;
	}
	else
		currentLoadContentCount_ = static_cast<float>(GetASyncLoadNum());
}

void DemoScene::Draw() const
{

	auto load = 100.0f - static_cast<float>(currentLoadContentCount_ / maxLoadContentCount_) * 100.0f;

	DrawFormatString(0, 0, GetColor(255, 255, 255), "DemoScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "Load... [%.1f]", load);
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
