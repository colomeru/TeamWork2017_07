#include "LaneChangeScreen.h"
#include"../../game/Random.h"
#include"../../Def.h"

LaneChangeScreen::LaneChangeScreen(World * world):world_(world), dir_(WindDir::UP), isCreateWind_(false)
{
	windDefPoses_[WindDir::UP] = WINDOW_HEIGHT;
	windDefPoses_[WindDir::DOWN] = 0;
}

void LaneChangeScreen::Init(WindDir dir)
{
	isCreateWind_ = true;
	timeCount_ = 0;
	dir_ = dir;
}

void LaneChangeScreen::Update()
{
	if (!isCreateWind_)return;
	timeCount_++;
	if (Random::GetInstance().Range(0, 100) <= 80)
	{
		Vector2 windNewPos = Vector2(Random::GetInstance().Range(0, WINDOW_WIDTH),windDefPoses_[dir_]);
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<LaneChangeWind>(world_, windNewPos, dir_));

	}
}

void LaneChangeScreen::Draw() const
{
}

void LaneChangeScreen::End()
{
	isCreateWind_ = false;
}
