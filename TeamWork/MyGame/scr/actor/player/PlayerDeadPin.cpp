#include "PlayerDeadPin.h"
#include"../../game/Random.h"
#include"../../math/MyFuncionList.h"

PlayerDeadPin::PlayerDeadPin(IWorld * world,const Vector2& position):
	Actor(world),sinCount_(0), gravity_(Vector2::Zero)
{
	direction_.x = Random::GetInstance().Range(-50.f, 50.f);
	direction_.y = Random::GetInstance().Range(-70.f, -40.f);
	position_ = position;
}

PlayerDeadPin::~PlayerDeadPin()
{
}

void PlayerDeadPin::Update()
{
	position_ += direction_;
	gravity_ += addgravity;
	direction_ += gravity_;

	angle_ = MathAngle(direction_)+180;

}

void PlayerDeadPin::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, GetDrawPosVect(position_), origin,1.f, Vector2::One,angle_);
}

void PlayerDeadPin::OnUpdate()
{
}

void PlayerDeadPin::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void PlayerDeadPin::OnMessage(EventMessage message, void * param)
{
}
