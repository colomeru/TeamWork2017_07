#include "UIScreen.h"

#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"../../game/Random.h"
#include"../../Def.h"
#include"../GamePlayDefine.h"
#include"screenSupport/DrawScore.h"
#include"../../tween/TweenManager.h"

static const int defTimeCount_ = 60;
static const int defGenerateCharaCount = 10;
static const int createWindCount = 100;

static const int defDrawPointYAdd[3] = { 0,30,60 };

UIScreen::UIScreen(World * world) :world_(world), timeCount_(0), meterPos_(Vector2(1100.0f, 50.0f)),
	meterLen_(800.0f)
{
}

void UIScreen::Init(Stage currentStage, float stageLen)
{
	stageLen_ = stageLen;
	currentStage_ = currentStage;
	fscore_ = 0.f;
	iscore_ = 0;
	meter_ = ProgressMeter(world_, stageLen,meterPos_);
	meter_.Initialize();
}

void UIScreen::Update(const Vector2& playerPos)
{
	playerPos_ = playerPos;
	meter_.Update();
}

void UIScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(1000, 0, 1920, 230, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	meter_.Draw();

	//DrawBox(meterPos_.x, meterPos_.y, meterPos_.x + meterLen_, meterPos_.y + 20, GetColor(0, 255, 0), 1);
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(playerPos_.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	
	//DrawScore::getInstance().Draw(Vector2(1000, 200), score_, 6);
	Vector2 origin=Sprite::GetInstance().GetSize(SPRITE_ID::BITECOUNT_SPRITE) / 2;
	int uipos=DrawScore::getInstance().Draw(Vector2(1900, 170), (int)roundf(fscore_), 5,Vector2(0.5f,0.5f));
	Sprite::GetInstance().Draw(SPRITE_ID::BITECOUNT_SPRITE, Vector2(uipos-origin.x, 170), origin, Vector2(0.5f, 0.5f));
}

void UIScreen::End()
{
}

void UIScreen::AddScore(int score)
{
	if ((int)roundf(fscore_) < iscore_) {
		TweenManager::GetInstance().Cancel(&fscore_);
		fscore_ = iscore_;
	}
	float toscore = iscore_ + score;
	toscore = min(toscore, 99999);
	iscore_ = (int)roundf(toscore);
	TweenManager::GetInstance().Add(EaseType::Linear, &fscore_, toscore, 0.5f);

}

