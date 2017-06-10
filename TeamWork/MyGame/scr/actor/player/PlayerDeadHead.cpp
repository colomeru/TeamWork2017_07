#include "PlayerDeadHead.h"

PlayerDeadHead::PlayerDeadHead(IWorld * world, const Vector2& position) :
	Actor(world), sinCount_(0)
{
	position_ = position;
}

PlayerDeadHead::~PlayerDeadHead()
{
}

void PlayerDeadHead::Update()
{
	sinCount_+=3;
	sinCount_ %= 360;
	position_.x = MathHelper::Sin(sinCount_)*150.f;
	position_.y -= 5.f;

	if (position_.y <= -600) {
		world_->sendMessage(EventMessage::PLAYER_DEAD);
		parameter_.isDead = true;
	}
}

void PlayerDeadHead::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::OROCHI_DEAD_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_DEAD_SPRITE, drawPos_, origin, Vector2::One);
}

void PlayerDeadHead::OnUpdate()
{
}

void PlayerDeadHead::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void PlayerDeadHead::OnMessage(EventMessage message, void * param)
{
}
