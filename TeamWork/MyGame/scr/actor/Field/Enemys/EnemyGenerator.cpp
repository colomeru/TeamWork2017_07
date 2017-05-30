#include "EnemyGenerator.h"
#include"EnemyCharas\Bird.h"

EnemyGenerator::EnemyGenerator(IWorld* world):
	Actor(world),timeCount_(0)
{
}

EnemyGenerator::~EnemyGenerator()
{
}

void EnemyGenerator::Update()
{
	timeCount_ = (timeCount_ + 1) % 300;
	if (timeCount_ == 0) {
		Vector2 generatePos = Vector2(WINDOW_WIDTH + world_->GetKeepDatas().playerPos_.x, -200);
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Bird>(world_, world_->GetKeepDatas().playerLane_, generatePos));
	}
}

void EnemyGenerator::Draw() const
{
}
void EnemyGenerator::OnUpdate()
{
}

void EnemyGenerator::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void EnemyGenerator::OnMessage(EventMessage message, void * param)
{
}
