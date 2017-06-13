#include "DemoScene.h"
#include "../fade/FadePanel.h"
#include "../tween/TweenManager.h"
#include"../graphic/Sprite.h"
#include"../Def.h"


static const int animDelay = 3;
static const int animSize = 8;
DemoScene::DemoScene() :
	isEnd_(false),
	maxLoadContentCount_((float)GetASyncLoadNum()),
	currentLoadCount_(0.0f),
	prevLoadCount_(0.0f)
{
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_1_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_2_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_3_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_4_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_5_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_6_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_7_SPRITE);
	wMachineFrame_.push_back(SPRITE_ID::LOAD_ANM_8_SPRITE);
}

DemoScene::~DemoScene()
{
}

void DemoScene::Initialize()
{
	isEnd_ = false;
	currentLoadCount_ = GetASyncLoadNum();

	timeCount_ = 0;
	LoadWashMachineFrame_ = 0;
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
	LoadWashMachineFrame_++; LoadWashMachineFrame_ %= animSize;

	if (currentLoadCount_ <= 0.1f && !isEnd_)
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

	Vector2 origin =Sprite::GetInstance().GetSize(SPRITE_ID::NOWLOADING_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::NOWLOADING_SPRITE, Vector2(1000, WINDOW_HEIGHT - origin.y), origin, Vector2::One);
	
	int loopCount = timeCount_/10;
	loopCount %= 4;
	for (int i = 0; i < loopCount; i++) {
		Vector2 dorigin = Sprite::GetInstance().GetSize(SPRITE_ID::DOT_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::DOT_SPRITE, Vector2(1000+origin.x+(i*2*dorigin.x), WINDOW_HEIGHT - dorigin.y), dorigin, Vector2::One);

	}

	Vector2 wmorigin = Sprite::GetInstance().GetSize(wMachineFrame_.at((int)roundf(currentLoadCount_/animDelay)%animSize)) / 2;
	Sprite::GetInstance().Draw(wMachineFrame_.at((int)roundf(currentLoadCount_/animDelay) % animSize), Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), wmorigin, Vector2::One);

	if (BuildMode != 1)return;
	auto load = 100.0f - (currentLoadCount_ / maxLoadContentCount_) * 100.0f;
	DrawFormatString(0, 100, GetColor(255, 255, 255), "NowLoadind... %.1f", load);
	DrawFormatString(0, 0, GetColor(255, 255, 255), "DemoScene");
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
	FadePanel::GetInstance().SetOutTime(0.f);
	FadePanel::GetInstance().SetInTime(0.3f);
	FadePanel::GetInstance().AddCollBack([=] {FadePanel::GetInstance().FadeIn(); });
	FadePanel::GetInstance().FadeOut();

}
