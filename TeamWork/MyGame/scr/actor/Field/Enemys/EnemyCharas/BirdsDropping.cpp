#include "BirdsDropping.h"
#include"../../../../math/Easing.h"

static const float defDropFallTime = 3.0f;

BirdsDropping::BirdsDropping(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), basePos_(pos), timeCount_(0.f)
{
	Vector2 rad = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE)/2;
	parameter_.radius = rad.x;

	colFuncMap_[COL_ID::DROP_PHEAD_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

BirdsDropping::~BirdsDropping()
{
}

void BirdsDropping::Update()
{
	position_.y = Easing::EaseInBack(timeCount_, basePos_.y, WINDOW_HEIGHT, defDropFallTime, 1.1f);
	timeCount_ += 0.016f;

	if (world_->GetKeepDatas().playerLane_ == laneNum_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::DROP_PHEAD_COL);
	}

	if (timeCount_ > defDropFallTime ||laneNum_!=world_->GetKeepDatas().playerLane_) {
		parameter_.isDead = true;
	}
}

void BirdsDropping::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::BIRDS_DROPPING_SPRITE, drawPos_, origin, spriteAlpha_, Vector2::One, 0);
}

void BirdsDropping::OnUpdate()
{
}

void BirdsDropping::OnCollide(Actor & other, CollisionParameter colpara)
{
	auto pH = static_cast<Player_Head*>(&other);

	if (pH->getIsCurrentHead()&&pH->GetPlayerPointer()->GetIsBiteMode()){
		pH->GetPlayerPointer()->SetMode(MODE_SLIP);
	}
}

void BirdsDropping::OnMessage(EventMessage message, void * param)
{
}
