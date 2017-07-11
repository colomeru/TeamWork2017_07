#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"screenSupport/BGCharas/BackgroundPill.h"
#include"../../game/Random.h"
#include"../../Def.h"
#include"../GamePlayDefine.h"
#include"../../debugdata/DebugDraw.h"

static const int defTimeCount_ = 60;
static const int defGenerateCharaCount = 10;
static const int createWindCount = 100;

static const int defDrawPointYAdd[3] = {0,30,60};

BackgroundScreen::BackgroundScreen(World * world) :world_(world),timeCount_(0)
{
	Vector2 cPos = Vector2(2000,0);
	for (int i = 0; i < defGenerateCharaCount;i++) {
		charactersPosition_.push_back(cPos);
		isCharactersPositionUsed_.push_back(false);

		cPos.x += 180;
		cPos.y += 100;
	}
	BGList_[Stage::Stage1] = SPRITE_ID::BACKGROUND_1_SPRITE;
	BGList_[Stage::Stage2] = SPRITE_ID::BACKGROUND_2_SPRITE;
	BGList_[Stage::Stage3] = SPRITE_ID::BACKGROUND_3_SPRITE;
	BGList_[Stage::Stage4] = SPRITE_ID::BACKGROUND_4_SPRITE;
	BGList_[Stage::Stage5] = SPRITE_ID::BACKGROUND_5_SPRITE;
	BGList_[Stage::Stage6] = SPRITE_ID::BACKGROUND_6_SPRITE;
	BGList_[Stage::Stage7] = SPRITE_ID::BACKGROUND_7_SPRITE;
	BGList_[Stage::Stage8] = SPRITE_ID::BACKGROUND_8_SPRITE;
}

void BackgroundScreen::Init(Stage currentStage)
{
	End();

	timeCount_ = 0;
	Vector2 pPos = world_->GetKeepDatas().playerPos_;
	currentStage_ = currentStage;
	
	 ceilPos = Vector2(0, -200.f);
}

void BackgroundScreen::Update()
{
	timeCount_++;
	if (createWindTimeAndPos_.size() <= 0)return;
	if (createWindTimeAndPos_.front().windTime_ == timeCount_) {
		Vector2 pPos = world_->GetKeepDatas().playerPos_;
		world_->Add(ACTOR_ID::BACK_GROUND_ACTOR, std::make_shared<BackgroundPill>(world_, createWindTimeAndPos_.front().windPos_ + pPos));

		createWindTimeAndPos_.pop_front();
	}
}

void BackgroundScreen::Draw() const
{
	Vector2 camPos(world_->GetInv().Translation().x/2,0);
	camPos.x = (int)camPos.x % (int)Sprite::GetInstance().GetSize(BGList_.at(currentStage_)).x;
	float myds=world_->GetKeepDatas().changeLaneLerpPos_;
	Vector2 addpos=Vector2::Lerp(Vector2::Zero,Vector2(0,30),myds) *world_->GetKeepDatas().nextLane_;
	Vector2 bgPos = Vector2(0,Sprite::GetInstance().GetSize(BGList_.at(currentStage_)).y-WINDOW_HEIGHT-30) - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	bgPos += (camPos);
	Vector2 BGSize = Vector2(Sprite::GetInstance().GetSize(BGList_.at(currentStage_)).x, 0);
	Sprite::GetInstance().Draw(BGList_.at(currentStage_), bgPos- BGSize);
	Sprite::GetInstance().Draw(BGList_.at(currentStage_), bgPos);
	Sprite::GetInstance().Draw(BGList_.at(currentStage_), BGSize+bgPos);

	Vector2 fencePos = Vector2(0, WINDOW_HEIGHT - Sprite::GetInstance().GetSize(SPRITE_ID::FENCE_SPRITE).y) - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	fencePos += (camPos)+Vector2(0.f, 60.f);
	Vector2 fenceSize = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::FENCE_SPRITE).x, 0);
	Sprite::GetInstance().Draw(SPRITE_ID::FENCE_SPRITE, fencePos- fenceSize);
	Sprite::GetInstance().Draw(SPRITE_ID::FENCE_SPRITE, fencePos);
	Sprite::GetInstance().Draw(SPRITE_ID::FENCE_SPRITE, fencePos+ fenceSize);
	
	Sprite::GetInstance().Draw(SPRITE_ID::CEILING_SPRITE, ceilPos);


	DebugDraw::DebugDrawFormatString(700, 700, GetColor(255, 255, 255), "%f", myds);
}

void BackgroundScreen::End()
{
	createWindTimeAndPos_.clear();
}

void BackgroundScreen::addBGCharacters()
{
	createWindTimeAndPos_.clear();
	timeCount_ = 0;
	for (auto& isC : isCharactersPositionUsed_) {
		isC = false;
	}


		for (int i = 0; i < createWindCount; i++) {

			int rand = Random::GetInstance().Range(0, 100);
			if (rand > 50) continue;

			int yPos = Random::GetInstance().Range(0, 9);

			int createTime = Random::GetInstance().Range(0, 400);
			createWindTimeAndPos_.push_back(WindTimeAndPos(createTime,Vector2(2000, charactersPosition_[yPos].y)));
		}
		createWindTimeAndPos_.sort();
}

void BackgroundScreen::DownCeil()
{
	ceilPos.y += 2;
	ceilPos.y = min(ceilPos.y,0);
}
