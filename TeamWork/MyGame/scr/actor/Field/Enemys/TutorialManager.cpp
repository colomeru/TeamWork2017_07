#include "TutorialManager.h"
#include"EnemyCharas\Bird.h"
#include"EnemyCharas\TutorialTapper.h"
#include"../../../Def.h"
#include"../../../time/Time.h"
#include"TutorialList.h"

static const int defWindCount = 400;
TutorialManager::TutorialManager(IWorld* world, int stage) :
	Actor(world), timeCount_(0), resurTimer_(0.0f), stage_(stage), windCount_(defWindCount){

	isResur_ = isSpawnTapperTutorial[stage_];
	if (isSpawnWind[stage_]) world_->sendMessage(EventMessage::BEGIN_WIND);
}

TutorialManager::~TutorialManager()
{
}

void TutorialManager::Update()
{
	resurTimer_ -= Time::DeltaTime;
	if (isResur_&&resurTimer_ <= 0.0f) {
		isResur_ = false;
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<TutorialTapper>(world_, world_->GetKeepDatas().playerLane_,Vector2((float)600, 0.f)));
	}
	if (isSpawnWind[stage_]) {
		windCount_--;
		if (windCount_ <= 0) {
			windCount_ = defWindCount;
			world_->sendMessage(EventMessage::BEGIN_WIND);
		}
	}
	if (spawnBirdTimeTutorial[stage_] == 0)return;
	timeCount_ = (timeCount_ + 1) % spawnBirdTimeTutorial[stage_];
	if (timeCount_ == 0) {
		Vector2 generatePos = Vector2(WINDOW_WIDTH + world_->GetKeepDatas().playerPos_.x, -200);
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Bird>(world_, world_->GetKeepDatas().playerLane_, generatePos));
	}
}

void TutorialManager::Draw() const
{
}
void TutorialManager::OnUpdate()
{
}

void TutorialManager::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void TutorialManager::OnMessage(EventMessage message, void * param)
{
}

void TutorialManager::StartTapperResurrectTimer()
{
	resurTimer_ = 10.0f;
	isResur_ = true;
}
