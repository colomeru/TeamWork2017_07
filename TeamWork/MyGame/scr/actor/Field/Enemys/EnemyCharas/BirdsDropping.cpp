#include "BirdsDropping.h"
#include"../../../../math/Easing.h"

static const float defDropTime = 3.0f;

BirdsDropping::BirdsDropping(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), basePos_(pos), timeCount_(0.f)
{

}

BirdsDropping::~BirdsDropping()
{
}

void BirdsDropping::Update()
{
	position_.y = Easing::EaseInBack(timeCount_, basePos_.y, WINDOW_HEIGHT, defDropTime, 1.1f);
	timeCount_ += 0.016f;

	if (timeCount_ > defDropTime) {
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
}

void BirdsDropping::OnMessage(EventMessage message, void * param)
{
}
