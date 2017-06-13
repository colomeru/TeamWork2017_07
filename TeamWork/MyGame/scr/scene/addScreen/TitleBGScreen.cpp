#include "TitleBGScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../fade/FadePanel.h"
#include"../../tween/TweenManager.h"
#include<algorithm>
#include"../../game/Random.h"


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

	lanePos_.push_back(-50);
	lanePos_.push_back(400);
	lanePos_.push_back(900);

	clotheslist_.push_back(SPRITE_ID::BASE_CLOTHES_SPRITE);
	clotheslist_.push_back(SPRITE_ID::BASE_CLOTHES_02_SPRITE);

	clothesAddPos_[SPRITE_ID::BASE_CLOTHES_SPRITE] = 125;
	clothesAddPos_[SPRITE_ID::BASE_CLOTHES_02_SPRITE] = 133;
}

void TitleBGScreen::Init()
{
	timeCount_ = 0;
	plusCount_ = 1;
	scrollPos_ = 0;
	whiteScreenAlpha_ = 0.f;
	whiteScreenAlphaAdd_ = 0.f;
	currentStage_ = 0;
	clothes_.clear();
	clothesCreateTime_ = 30;
}

void TitleBGScreen::Update()
{
	timeCount_ += plusCount_;

	clothesCreateTime_--;
	if (clothesCreateTime_ <= 0) {
		clothesCreateTime_ = Random::GetInstance().Range(20, 60);
		clothes_.push_back(TitleLaneClothes(clotheslist_[Random::GetInstance().Range(0,clotheslist_.size())], Random::GetInstance().Range(0,3), WINDOW_WIDTH+100));
	}

	
	for (auto& i : clothes_) {
		i.lanePos -= 10;
	}
	auto clothes_itr = std::remove_if(clothes_.begin(), clothes_.end(), [](TitleLaneClothes i) {return i.lanePos <= -200; });

	clothes_.erase(clothes_itr, clothes_.end());

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

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::TITLE_POLE_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_POLE_SPRITE, Vector2(WINDOW_WIDTH / 2, lanePos_.at(1)), origin, Vector2::One);
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_POLE_SPRITE, Vector2(WINDOW_WIDTH / 2, lanePos_.at(2)), origin, Vector2::One);

	//DrawClothes(1, 300,SPRITE_ID::BASE_CLOTHES_02_SPRITE);

	for (auto i : clothes_) {
		DrawClothes(i.laneNum, i.lanePos, i.drawID);
	}

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

void TitleBGScreen::DrawClothes(int laneNum, int xpos, SPRITE_ID clothestype)const
{
	Vector2 hangOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, Vector2(xpos, lanePos_.at(laneNum)+25), hangOrigin, 1, Vector2::One, 0);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(clothestype) / 2;
	Sprite::GetInstance().SplitDraw(clothestype, Vector2(xpos, lanePos_.at(laneNum)+ clothesAddPos_.at(clothestype)), 0, crcOrigin, 1, Vector2::One, 0);

}
