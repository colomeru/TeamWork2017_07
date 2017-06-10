#include "DemoScene.h"
#include "../fade/FadePanel.h"
#include "../tween/TweenManager.h"
#include"../graphic/Sprite.h"

DemoScene::DemoScene() :
	isEnd_(false),
	maxLoadContentCount_((float)GetASyncLoadNum()),
	currentLoadCount_(0.0f),
	prevLoadCount_(0.0f)
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::Initialize()
{
	isEnd_ = false;
	currentLoadCount_ = GetASyncLoadNum();

	timeCount_ = 0;

	// フェードイン
	FadePanel::GetInstance().Initialize();
	FadePanel::GetInstance().SetInTime(0.0f);
	FadePanel::GetInstance().FadeIn();
}

void DemoScene::Update()
{
	if (!FadePanel::GetInstance().IsClearScreen())
		return;

	timeCount_++;

	if (currentLoadCount_ <= 0.0f && !isEnd_)
	{
		FadePanel::GetInstance().AddCollBack([=]() {
			SetBackgroundColor(153, 204, 255);
			isEnd_ = true;
		});
		FadePanel::GetInstance().SetOutTime(1.0f);
		FadePanel::GetInstance().FadeOut();
	}

	if (prevLoadCount_ > GetASyncLoadNum())
		TweenManager::GetInstance().Add(EaseOutCirc, &currentLoadCount_, (float)GetASyncLoadNum());

	prevLoadCount_ = GetASyncLoadNum();
}

void DemoScene::Draw() const
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "DemoScene");

	Vector2 origin =Sprite::GetInstance().GetSize(SPRITE_ID::NOWLOADING_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::NOWLOADING_SPRITE, Vector2(1000, WINDOW_HEIGHT - origin.y), origin, Vector2::One);
	
	int loopCount = timeCount_/10;
	loopCount %= 4;
	for (int i = 0; i < loopCount; i++) {
		Vector2 dorigin = Sprite::GetInstance().GetSize(SPRITE_ID::DOT_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::DOT_SPRITE, Vector2(1000+origin.x+(i*2*dorigin.x), WINDOW_HEIGHT - dorigin.y), dorigin, Vector2::One);

	}
	auto load = 100.0f - (currentLoadCount_ / maxLoadContentCount_) * 100.0f;
	DrawFormatString(0, 100, GetColor(255, 255, 255), "NowLoadind... %.1f", load);
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
