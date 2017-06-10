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
	score_ = 0;
	fscore_ = 0.f;
}

void UIScreen::Update(const Vector2& playerPos)
{
	playerPos_ = playerPos;
}

void UIScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(1000, 0, 1920, 200, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(meterPos_.x, meterPos_.y, meterPos_.x + meterLen_, meterPos_.y + 20, GetColor(0, 255, 0), 1);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(playerPos_.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	
	//DrawScore::getInstance().Draw(Vector2(1000, 200), score_, 6);
	DrawScore::getInstance().Draw(Vector2(1000, 200), (int)roundf(fscore_), 6);
}

void UIScreen::End()
{
}

void UIScreen::AddScore(int score)
{
	float toscore = fscore_ + score;
	toscore = min(toscore, 999999);

	TweenManager::GetInstance().Add(EaseType::Linear, &fscore_, toscore, 0.5f);

}

