#include "Player_Sword.h"
#include"../../math/MyFuncionList.h"
#include"../../debugdata/DebugDraw.h"
#include "../../graphic/Sprite.h"

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

	DebugDraw::DebugDrawLine((int)startDP.x, (int)startDP.y, (int)endDP.x, (int)endDP.y, GetColor(0, 0, 255));
	DebugDraw::DebugDrawFormatString(400, 400, GetColor(255, 255, 255), "%f", angle);
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

void Player_Sword::SetSwordVel(const Vector2 & vel) {
	position_ = player_->GetPosition();
	swordEndPos_ = position_ + (vel*(float)Sprite::GetInstance().GetSize(SPRITE_ID::SWORD_SPRITE).y);

	swordStartPos_ = position_ + (swordEndPos_ - position_).Normalize()*128.f;

}

bool Player_Sword::CamMoveUpdate() {
	if (world_->GetKeepDatas().nextLane_ < 0) {
		CamMoveUp();
	}
	else {
		CamMoveDown();
	}
	return true;
}

void Player_Sword::CamMoveUp() {
}

void Player_Sword::CamMoveDown() {
	if (!player_->isLaneChangeFall()) {
		return;
	}
	LaneChangeFall();
	drawPos_ = GetDrawPosVect(position_);

}

void Player_Sword::LaneChangeFall() {
	float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
	laneLerpNum = min(1.f, laneLerpNum);
	int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
	drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

	if (player_->isLaneChangeFall()) {
		drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
	}
}

Vector2 Player_Sword::GetSwordStartPos() const {
	return swordStartPos_;
}

Vector2 Player_Sword::GetSwordEndPos() const {
	return swordEndPos_;
}

void Player_Sword::SetUseSword(bool useSword) {
	useSword_ = useSword;
}

bool Player_Sword::GetUseSword() const {
	return useSword_;
}
