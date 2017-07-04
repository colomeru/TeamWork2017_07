#include "TitleBGScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../fade/FadePanel.h"
#include"../../tween/TweenManager.h"
#include<algorithm>
#include"../../game/Random.h"
#include"../../math/MyFuncionList.h"

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

	lanePos_[0]=-50;
	lanePos_[1]=400;
	lanePos_[2]=900;

	clotheslist_[0]=SPRITE_ID::BASE_CLOTHES_SPRITE;
	clotheslist_[1] = SPRITE_ID::BASE_CLOTHES_02_SPRITE;
	clotheslist_[2] = SPRITE_ID::BASE_CLOTHES_03_SPRITE;
	clotheslist_[3] = SPRITE_ID::BASE_CLOTHES_04_SPRITE;
	clotheslist_[4] = SPRITE_ID::BASE_CLOTHES_05_SPRITE;
	clotheslist_[5] = SPRITE_ID::BASE_CLOTHES_06_SPRITE;
	clotheslist_[6] = SPRITE_ID::BASE_CLOTHES_07_SPRITE;
	clotheslist_[7] = SPRITE_ID::BASE_CLOTHES_08_SPRITE;
	clotheslist_[8] = SPRITE_ID::BASE_CLOTHES_09_SPRITE;
	clotheslist_[9] = SPRITE_ID::BASE_CLOTHES_10_SPRITE;
	clotheslist_[10] = SPRITE_ID::BASE_CLOTHES_11_SPRITE;
	clotheslist_[11] = SPRITE_ID::BASE_CLOTHES_12_SPRITE;
	clotheslist_[12] = SPRITE_ID::BASE_CLOTHES_13_SPRITE;
	clotheslist_[13] = SPRITE_ID::BASE_CLOTHES_14_SPRITE;
	clotheslist_[14] = SPRITE_ID::BASE_CLOTHES_15_SPRITE;
	clotheslist_[15] = SPRITE_ID::BASE_CLOTHES_16_SPRITE;
	clotheslist_[16] = SPRITE_ID::BASE_CLOTHES_17_SPRITE;
	clotheslist_[17] = SPRITE_ID::BASE_CLOTHES_18_SPRITE;
	clotheslist_[18]=SPRITE_ID::BASE_CLOTHES_19_SPRITE;

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

	clothesCreateTime_-=Random::GetInstance().Range(1,3);
	if (clothesCreateTime_ <= 0) {
		clothesCreateTime_ = Random::GetInstance().Range(20, 60);
		auto clothestype= clotheslist_[Random::GetInstance().Range(0, clotheslist_.size())];
		int laneNum= Random::GetInstance().Range(0, 3);
		Vector2 origin=Sprite::GetInstance().GetSplitPieceSize(clothestype)/2;
		origin.x = 0.f;
		clothes_.push_back(TitleLaneClothes(clothestype, laneNum, WINDOW_WIDTH+100,20.f,Random::GetInstance().Range(1.f,90.f),Random::GetInstance().Range(2.f,4.f), origin));

	}

	
	for (auto& i : clothes_) {
		i.lanePos -= 10;
		i.sinCount_ += i.windPower; i.sinCount_ = fmodf(i.sinCount_, 360.f);
		i.rotate = MathHelper::Sin(i.sinCount_)*i.maxAngle;
	}
	auto clothes_itr = std::remove_if(clothes_.begin(), clothes_.end(), [](TitleLaneClothes i) {return i.lanePos <= -200; });

	clothes_.erase(clothes_itr, clothes_.end());

	if (timeCount_ >= 300) {
		TweenManager::GetInstance().Add(Linear, &whiteScreenAlpha_, 1.f, 0.2f, [=] {WhiteScreenMinus(); });
		timeCount_ = 0;
	}

	scrollPos_+=5;scrollPos_ %= WINDOW_WIDTH;
}
void TitleBGScreen::Draw() const
{

	Sprite::GetInstance().Draw(BGList_.at(currentStage_), Vector2::Zero-Vector2(scrollPos_,0));
	Sprite::GetInstance().Draw(BGList_.at(currentStage_), Vector2(WINDOW_WIDTH,0) - Vector2(scrollPos_, 0));

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::TITLE_POLE_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_POLE_SPRITE, Vector2(WINDOW_WIDTH / 2, lanePos_[1]), origin, Vector2::One);
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_POLE_SPRITE, Vector2(WINDOW_WIDTH / 2, lanePos_[2]), origin, Vector2::One);

	for (auto i : clothes_) {
		DrawClothes(i);
	}

	Sprite::GetInstance().Draw(SPRITE_ID::WHITE_SCREEN_SPRITE, Vector2::Zero, whiteScreenAlpha_);
}

void TitleBGScreen::End()
{
}

void TitleBGScreen::WhiteScreenMinus(){

	TweenManager::GetInstance().Add(Linear, &whiteScreenAlpha_, 0.f, 0.2f);

	currentStage_++; currentStage_ %= BGList_.size();
}

void TitleBGScreen::DrawClothes(const TitleLaneClothes& clothes)const
{
	Vector2 hangOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE) / 2;
	hangOrigin.y = 0;
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(clothes.drawID) / 2;
	crcOrigin.y = 0;

	Sprite::GetInstance().SplitDraw(clothes.drawID, Vector2(clothes.lanePos, lanePos_[clothes.laneNum]), 0, crcOrigin, 1, Vector2::One, clothes.rotate);

}

