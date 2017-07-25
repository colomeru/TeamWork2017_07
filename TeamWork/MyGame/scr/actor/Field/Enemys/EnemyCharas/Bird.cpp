#include "Bird.h"
#include"BirdsDropping.h"
#include "../../../../game/Random.h"
#include "../../../../graphic/Sprite.h"

Bird::Bird(IWorld * world, int laneNum, Vector2 pos) :
	Enemys(world, laneNum, pos),dropTimer_(0), timeCount_(0.0f), basePos_(pos),
	defDropTime(Random::GetInstance().Range(150, 200))
{
	anmManager_.Add(SPRITE_ID::CROW_ANM_01_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_02_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_03_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_04_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_05_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_06_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_07_SPRITE);
	anmManager_.Add(SPRITE_ID::CROW_ANM_08_SPRITE);
	anmManager_.SetIsRepeat(true);
}

Bird::~Bird()
{
}

void Bird::Update()
{
	timeCount_ += 0.016f;

	dropTimer_ =(dropTimer_ +1)% defDropTime;
	
	position_.x -= 5.f;

	if (dropTimer_ == 0) {
			world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<BirdsDropping>(world_, laneNum_, position_));
	}
	if (position_.x <= world_->GetKeepDatas().playerPos_.x-WINDOW_WIDTH|| laneNum_ != world_->GetKeepDatas().playerLane_) {
		parameter_.isDead = true;
	}
	anmManager_.Update();
}

void Bird::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(drawPos_, origin,Vector2::One, parameter_.spriteAlpha_);
}

void Bird::OnUpdate()
{
}

void Bird::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Bird::OnMessage(EventMessage message, void * param)
{
}
