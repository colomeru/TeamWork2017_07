#include "TitleScene.h"
#include "../Def.h"
#include "../time/Time.h"

// èâä˙âª
void TitleScene::Initialize()
{
	isEnd_ = false;
}

void TitleScene::Update()
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;

}

void TitleScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "TitleScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
}

bool TitleScene::IsEnd() const
{
	return isEnd_;
}

Scene TitleScene::Next() const
{
	return Scene::GamePlay;
}

void TitleScene::End()
{
}
