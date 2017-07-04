#include "Player_Sword.h"
#include"../../math/MyFuncionList.h"

Player_Sword::Player_Sword(IWorld * world, Player * targetP, Vector2 pos)
	:Actor(world,targetP),swordStartPos_(pos),swordEndPos_(pos),player_(targetP),useSword_(false)
{
	parameter_.ID = ACTOR_ID::PLAYER_SWORD_ACTOR;
	position_ = pos;
	laneNum_ = targetP->GetLaneNum();
	colFuncMap_[COL_ID::PSWORD_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_PSword_Clothes, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

Player_Sword::~Player_Sword()
{
}

void Player_Sword::Update()
{
	if (isCheckCol_&&isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::PSWORD_CLOTHES_COL);
	}
}

void Player_Sword::Draw() const
{
	if (!GetUseSword())return;
	Vector2 startDP = GetDrawPosVect(swordStartPos_);
	Vector2 endDP = GetDrawPosVect(swordEndPos_);
	auto pos = GetDrawPosVect(position_);
	Vector2 vel = endDP- pos;
	float angle = MathAngle(vel);
	Vector2 origin((float)Sprite::GetInstance().GetSize(SPRITE_ID::SWORD_SPRITE).x/2.f,0);
	Sprite::GetInstance().Draw(SPRITE_ID::SWORD_SPRITE, pos,origin, Vector2::One, angle);

	if (BuildMode != 1)return;
	DrawLine((int)startDP.x, (int)startDP.y, (int)endDP.x, (int)endDP.y, GetColor(0, 0, 255));
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
