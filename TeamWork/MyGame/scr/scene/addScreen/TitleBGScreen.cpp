#include "TitleBGScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../fade/FadePanel.h"
#include"../../tween/TweenManager.h"



// コンストラクタ

TitleBGScreen::TitleBGScreen() {
	BGList_[0] = SPRITE_ID::BACKGROUND_1_SPRITE;
	BGList_[1] = SPRITE_ID::BACKGROUND_2_SPRITE;
	BGList_[2] = SPRITE_ID::BACKGROUND_3_SPRITE;
	BGList_[3] = SPRITE_ID::BACKGROUND_4_SPRITE;
	BGList_[4] = SPRITE_ID::BACKGROUND_5_SPRITE;
	BGList_[5] = SPRITE_ID::BACKGROUND_6_SPRITE;
	BGList_[6] = SPRITE_ID::BACKGROUND_7_SPRITE;
	BGList_[7] = SPRITE_ID::BACKGROUND_8_SPRITE;

	lanePos_.push_back(-200);
	lanePos_.push_back(400);
	lanePos_.push_back(900);
}

void TitleBGScreen::Init()
{
	timeCount_ = 0;
	plusCount_ = 1;
	scrollPos_ = 0;
	whiteScreenAlpha_ = 0.f;
	whiteScreenAlphaAdd_ = 0.f;
	currentStage_ = 0;
}

void TitleBGScreen::Update()
{
	timeCount_ += plusCount_;
	if (timeCount_ >= 300) {
		TweenManager::GetInstance().Add(Linear, &whiteScreenAlpha_, 1.f, 0.2f, [=] {WhiteScreenMinus(); });
		timeCount_ = 0;
	}

	whiteScreenAlpha_ += whiteScreenAlphaAdd_;
	if (whiteScreenAlpha_ <= 0.f) {
		whiteScreenAlphaAdd_ = 0.f;
		whiteScreenAlpha_ = 0.f;
	}
	scrollPos_+=5;scrollPos_ %= WINDOW_WIDTH;
}
void TitleBGScreen::Draw() const
{

	Sprite::GetInstance().Draw(BGList_.at(currentStage_), Vector2::Zero-Vector2(scrollPos_,0));
	Sprite::GetInstance().Draw(BGList_.at(currentStage_), Vector2(WINDOW_WIDTH,0) - Vector2(scrollPos_, 0));

	//Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE),

	Sprite::GetInstance().Draw(SPRITE_ID::WHITE_SCREEN_SPRITE, Vector2::Zero, whiteScreenAlpha_);
}

void TitleBGScreen::End()
{
}

void TitleBGScreen::WhiteScreenMinus(){

	//TweenManager::GetInstance().Cancel(&whiteScreenAlpha_);
	//TweenManager::GetInstance().Add(Linear, &whiteScreenAlpha_, 0.f, 0.2f);

	whiteScreenAlphaAdd_ = -0.05f;
	currentStage_++; currentStage_ %= BGList_.size();
}
