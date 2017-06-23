#include "CreditTextGenerator.h"
#include "../../../tween/TweenManager.h"
#include "CreditText.h"
#include "../../../time/Time.h"

CreditTextGenerator::CreditTextGenerator(IWorld * world, Vector2 position) :
	Actor(world)
{
	position_ = position;
	timer_ = 0.0f;
}

CreditTextGenerator::~CreditTextGenerator()
{
}

void CreditTextGenerator::Update()
{
	timer_ += Time::DeltaTime;

	if (timer_ >= DefTime) {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES,world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH, 0.0f)));
		timer_ = 0.0f;
	}


}

void CreditTextGenerator::Draw() const
{
}
