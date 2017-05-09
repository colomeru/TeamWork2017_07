#include "HairballGenerator.h"
#include "../Hairball.h"

HairballGenerator::HairballGenerator(IWorld * world)
	:Actor(world)
{
	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [&, this](const Actor& other) {
		player_ = const_cast<Actor*>(&other);
	});
}

HairballGenerator::~HairballGenerator()
{
}

void HairballGenerator::Update()
{
	position_ = Vector2(player_->GetPosition().x + 1600, player_->GetPosition().y);
}

void HairballGenerator::Draw() const
{
	//DrawFormatString(700, 160, GetColor(255, 255, 255), "pos.x:%f pos.y:%f", position_.x, position_.y);
}

void HairballGenerator::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
	{
		world_->Add(ACTOR_ID::EFECT_ACTOR, std::make_shared<Hairball>(world_, CLOTHES_ID::HAIRBALL, player_->GetLaneNum(), position_));
	}
	default:
		break;
	}
}
