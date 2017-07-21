#include "BirdsDropping.h"
#include"../../../../math/Easing.h"
#include"../../../../tween/TweenManager.h"

static const float defDropFallTime = 3.0f;

BirdsDropping::BirdsDropping(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos)
{
	Vector2 rad = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE)/2;
	parameter_.radius = rad.x;

	parameter_.ID = ENEMY_ACTOR;

	colFuncMap_[COL_ID::DROP_PHEAD_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);
	
	colFuncMap_[COL_ID::DROP_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_Droping_Clothes, colFunc_, std::placeholders::_1, std::placeholders::_2);


	TweenManager::GetInstance().Add(EaseType::EaseInExpo, &position_.y, (float)WINDOW_HEIGHT, defDropFallTime, [this] {parameter_.isDead = true; });

}

BirdsDropping::~BirdsDropping()
{
}

void BirdsDropping::FastUpdate() {
	isUpdate_ = true;
}

void BirdsDropping::Update()
{

	if (world_->GetKeepDatas().playerLane_ == laneNum_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::DROP_PHEAD_COL);
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::DROP_CLOTHES_COL);
		
	}

}

void BirdsDropping::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::BIRDS_DROPPING_SPRITE, drawPos_, origin, parameter_.spriteAlpha_, Vector2::One, 0);
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
