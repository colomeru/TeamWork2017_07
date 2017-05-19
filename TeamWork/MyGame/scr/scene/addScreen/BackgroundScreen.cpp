#include "BackgroundScreen.h"
#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"screenSupport/BGCharas/BackgroundPill.h"
#include"../../game/Random.h"
#include"../../Def.h"
#include"../GamePlayDefine.h"

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
		cPos.y += 150;
	}
}

void BackgroundScreen::Init()
{
	timeCount_ = 0;
	Vector2 pPos = world_->GetKeepDatas().playerPos_;
	//characters_.push_back(new BackgroundPill(world_ , Vector2(pPos.x+2000, 500)));
	//characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000, 200)));
	//characters_.push_back(new BackgroundPill(world_,Vector2(pPos.x+2000,700)));

}

void BackgroundScreen::Update()
{
	timeCount_++;
	if (createWindTimeAndPos_.find(timeCount_)!= createWindTimeAndPos_.end()) {
		Vector2 pPos = world_->GetKeepDatas().playerPos_;
		world_->Add(ACTOR_ID::BACK_GROUND_ACTOR, std::make_shared<BackgroundPill>(world_, createWindTimeAndPos_[timeCount_]+pPos));

	}
	//std::vector<BackgroundCharacters*>::iterator itr = characters_.begin();
	//while (itr != characters_.end())
	//{
	//	//size_t index = std::distance(characters_.begin(), itr);
	//	//characters_[index]->Update();
	//	(*itr)->Update();
	//	if ((*itr)->isDead()) {
	//		itr = characters_.erase(itr);
	//	}
	//	else {
	//		++itr;
	//	}
	//}
	
	//for (auto& bgC : characters_) {
	//	bgC->Update();
	//	if (bgC->isDead());
	//}
}

void BackgroundScreen::Draw() const
{
	float myds=world_->GetKeepDatas().changeLaneLerpPos_;
	Vector2 addpos=Vector2::Lerp(Vector2::Zero,Vector2(0,30),myds) *world_->GetKeepDatas().nextLane_;
	Vector2 bgPos = Vector2(0,Sprite::GetInstance().GetSize(SPRITE_ID::BACKGROUND_SPRITE).y-WINDOW_HEIGHT) - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	Sprite::GetInstance().Draw(SPRITE_ID::BACKGROUND_SPRITE, bgPos);

	Vector2 fencePos = Vector2(0, WINDOW_HEIGHT - Sprite::GetInstance().GetSize(SPRITE_ID::FENCE_SPRITE).y) - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_]);
	Sprite::GetInstance().Draw(SPRITE_ID::FENCE_SPRITE, fencePos);
	
	Vector2 ceilPos = Vector2::Zero - addpos - Vector2(0, defDrawPointYAdd[world_->GetKeepDatas().playerLane_] +Sprite::GetInstance().GetSize(SPRITE_ID::CEILING_SPRITE).y- defDrawPointYAdd[1]);
	Sprite::GetInstance().Draw(SPRITE_ID::CEILING_SPRITE, ceilPos);


	//for (auto& bgC : characters_) {
	//	bgC->Draw();
	//}
	DrawFormatString(700, 700, GetColor(255, 255, 255), "%f", myds);
}

void BackgroundScreen::End()
{

}

void BackgroundScreen::addBGCharacters()
{
	//timeCount_--;
	//if (timeCount_ <= 0) {
	//	timeCount_ = defTimeCount_;
		
	createWindTimeAndPos_.clear();

	for (auto& isC : isCharactersPositionUsed_) {
		isC = false;
	}


		for (int i = 0; i < createWindCount; i++) {

			int rand = Random::GetInstance().Range(0, 100);
			if (rand > 50) continue;

			int yPos = Random::GetInstance().Range(0, 9);


			//charactersPosition_[i];
			//isCharactersPositionUsed_[i]=true;

			//characters_.push_back(new BackgroundPill(world_, Vector2(2000+(i*100),charactersPosition_[yPos].y)+pPos));
			//world_->Add(ACTOR_ID::BACK_GROUND_ACTOR, std::make_shared<BackgroundPill>(world_, Vector2(2000 + (i * 100), charactersPosition_[yPos].y) + pPos));

			int createTime = Random::GetInstance().Range(0, defWindTime_);

			createWindTimeAndPos_[createTime] = Vector2(2000, charactersPosition_[yPos].y);
		}

		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2000, -200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2300, 0)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2500, 200)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2600, -400)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 2900, 300)));
		//characters_.push_back(new BackgroundPill(world_, Vector2(pPos.x + 3200, 0)));
		//}
}
