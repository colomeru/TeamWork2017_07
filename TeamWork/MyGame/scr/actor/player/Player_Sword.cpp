#include "Player_Sword.h"
#include"../../math/MyFuncionList.h"

Player_Sword::Player_Sword(IWorld * world, Player * targetP, Vector2 pos)
	:Actor(world),swordStartPos_(pos),swordEndPos_(pos),player_(targetP),useSword_(false)
{
	position_ = pos;
	laneNum_ = targetP->GetLaneNum();
}

Player_Sword::~Player_Sword()
{
}

void Player_Sword::Update()
{
}

void Player_Sword::Draw() const
{
	if (!GetUseSword())return;
	Vector2 startDP = GetDrawPosVect(swordStartPos_);
	Vector2 endDP = GetDrawPosVect(swordEndPos_);

	Vector2 vel = endDP-drawPos_;
	float angle = MathAngle(vel);
	Vector2 origin(Sprite::GetInstance().GetSize(SPRITE_ID::SWORD_SPRITE).x/2,0);
	Sprite::GetInstance().Draw(SPRITE_ID::SWORD_SPRITE, drawPos_,origin, Vector2::One, angle);

	if (BuildMode != 1)return;
	DrawLine(startDP.x, startDP.y, endDP.x, endDP.y, GetColor(0, 0, 255));
	DrawFormatString(400, 400, GetColor(255, 255, 255), "%f", angle);
}

void Player_Sword::OnUpdate()
{
}

void Player_Sword::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Player_Sword::OnMessage(EventMessage message, void * param)
{
}
