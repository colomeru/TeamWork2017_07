#include "EnemyGenerator.h"
#include"EnemyCharas\Bird.h"
#include"EnemyCharas\ClothesTapper.h"
#include"../../../Def.h"
#include"../../../time/Time.h"
#include"../../../scene/GamePlayDefine.h"

EnemyGenerator::EnemyGenerator(IWorld* world,Stage stage):
	Actor(world),timeCount_(0),resurTimer_(0.0f),stage_(stage){
	slist_[Stage::Stage1] = 0;
	slist_[Stage::Stage2] = 1;
	slist_[Stage::Stage3] = 2;
	slist_[Stage::Stage4] = 3;
	slist_[Stage::Stage5] = 4;
	slist_[Stage::Stage6] = 5;
	slist_[Stage::Stage7] = 6;
	slist_[Stage::Stage8] = 7;
	
	isResur_ = isSpawnTapper[slist_[stage]];
}

EnemyGenerator::~EnemyGenerator()
{
}

void EnemyGenerator::Update()
{
	timeCount_ = (timeCount_ + 1) % spawnBirfdTime[slist_[stage_]];
	if (timeCount_ == 0) {
		Vector2 generatePos = Vector2(WINDOW_WIDTH + world_->GetKeepDatas().playerPos_.x, -200);
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Bird>(world_, world_->GetKeepDatas().playerLane_, generatePos));
	}
	resurTimer_ -= Time::DeltaTime;
	if (isResur_&&resurTimer_ <= 0.0f) {
		isResur_ = false;
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<ClothesTapper>(world_, world_->GetKeepDatas().playerLane_, world_->GetKeepDatas().playerPos_+Vector2(WINDOW_WIDTH,0.f)));

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

void EnemyGenerator::StartTapperResurrectTimer()
{
	resurTimer_ = 10.0f;
	isResur_ = true;
}
