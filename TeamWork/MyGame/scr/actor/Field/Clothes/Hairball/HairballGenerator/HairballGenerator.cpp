#include "HairballGenerator.h"
#include "../Hairball.h"
#include "../MyGame/scr/Def.h"
#include "../MyGame/scr/game/Random.h"

HairballGenerator::HairballGenerator(IWorld * world, int laneNum, Vector2 pos)
	:Actor(world)
{
	laneNum_ = laneNum;
	position_ = pos;

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [&, this](const Actor& other) {
		player_ = const_cast<Actor*>(&other);
	});
}

HairballGenerator::~HairballGenerator()
{
}

void HairballGenerator::Update()
{
	if (player_ == nullptr)return;

	if (player_->GetLaneNum() == laneNum_)
		position_ = Vector2(player_->GetPosition().x + WINDOW_WIDTH, player_->GetPosition().y);
	else
		position_.x = player_->GetPosition().x + WINDOW_WIDTH;
}

void HairballGenerator::Draw() const
{
}

void HairballGenerator::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
	{
		int rand = Random::GetInstance().Range(0, 100);
		if (rand > 30) return;
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<Hairball>(world_, CLOTHES_ID::HAIRBALL, laneNum_, position_));
	}
	default:
		break;
	}
}
