#include "Bird.h"
#include"BirdsDropping.h"
#include"../../../../math/Easing.h"
#include"../../../../game/Random.h"

static int defDropTime = 60;
Bird::Bird(IWorld * world, int laneNum, Vector2 pos) :
	Enemys(world, laneNum, pos),dropTimer_(0), timeCount_(0.0f), basePos_(pos)
{

}

Bird::~Bird()
{
}

void Bird::Update()
{
	timeCount_ += 0.016f;

	dropTimer_ =(dropTimer_ +1)% defDropTime;
	
	//position_.x = Easing::EaseInCirc(timeCount_, basePos_.x, -WINDOW_WIDTH, defDropTime);
	position_.x -= 5.f;

	if (dropTimer_ == 0) {
		//if(Random::GetInstance().Range(0,100)>30)
			world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<BirdsDropping>(world_, laneNum_, position_));
	}
	if (position_.x <= world_->GetKeepDatas().playerPos_.x-WINDOW_WIDTH) {
		parameter_.isDead = true;
	}
}

void Bird::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::BIRD_SPRITE, drawPos_, origin, spriteAlpha_, Vector2::One, 0);
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
