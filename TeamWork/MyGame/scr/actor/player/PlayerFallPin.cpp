#include "PlayerFallPin.h"
#include "../../game/Random.h"
#include "../../math/MyFuncionList.h"
#include "../../graphic/Sprite.h"

PlayerFallPin::PlayerFallPin(IWorld * world, const Vector2& position,const Vector2& toPos) :
	Actor(world), sinCount_(0), gravity_(Vector2::Zero), direction_((toPos - position).Normalize())
{
	laneNum_ = world->GetKeepDatas().playerLane_;
	position_ = position;
}

PlayerFallPin::~PlayerFallPin()
{
}

void PlayerFallPin::Update()
{
	position_ += direction_;
	gravity_ += addgravity;
	direction_ += gravity_;

	angle_ = MathAngle(direction_) + 180;

}

void PlayerFallPin::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, GetDrawPosVect(position_), origin, 1.f, Vector2::One, angle_);
}

void PlayerFallPin::OnUpdate()
{
}

void PlayerFallPin::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void PlayerFallPin::OnMessage(EventMessage message, void * param)
{
}
