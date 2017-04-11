#include "AdhereMiniBoss.h"
#include "ImportAnimationNumber.h"
#include "../../../../../ResourceLoader/ResourceLoader.h"
#include "../../../../../Define.h"
#include "../../../../../World/IWorld.h"
#include <random>

AdhereMiniBoss::AdhereMiniBoss(
	IWorld * world, const Vector2 & position, 
	const float bodyScale) :
	FighterMiniBoss(world, position, bodyScale),
	adhereDeadTime_(4.0f),
	dState_(DeadState::HitDead)
{
}

void AdhereMiniBoss::changeDeadState(DeadState dState, int animation)
{
	changeState(State::Dead, animation);
	if (dState_ == dState) return;
	dState_ = dState;
}

void AdhereMiniBoss::idel(float deltaTime)
{
	if (stateTimer_ < 0.5f) return;
	changeState(State::Move, WAIT_NUMBER);
	auto player = world_->findActor("PlayerBody1");
	auto distance = player->getPosition() - position_;
	prevPlayerDirection_ = distance.Normalize();
}

void AdhereMiniBoss::move(float deltaTime){}

void AdhereMiniBoss::attack(float deltaTime)
{
	auto player = world_->findActor(playerName_);
	// プレイヤーがいないなら死亡
	if (player == nullptr) {
		changeDeadState(DeadState::HitDead, DEAD_NUMBER);
		body_.enabled(false);
		return;
	}
	auto addPos = Vector2(
		100.0f * MathHelper::Cos(degree_),
		100.0f * MathHelper::Sin(degree_));
	position_ = player->getPosition() + addPos;
	addTexPos_ = Vector2::Up * 10.0f;
	if (stateTimer_ < adhereDeadTime_) return;
	changeDeadState(DeadState::AdhereDead, DEAD_NUMBER);
	body_.enabled(false);
}

void AdhereMiniBoss::deadMove(float deltaTime){

	switch (dState_)
	{
	case DeadState::HitDead: FighterMiniBoss::deadMove(deltaTime); break;
	case DeadState::AdhereDead: adhereDead(deltaTime); break;
	}
}

// くっつき死亡状態
void AdhereMiniBoss::adhereDead(float deltaTime)
{
	name_ = "DeadMiniBoss";
	size_ -= deltaTime;
	size_ = max(0.0f, size_);
	if (size_ == 0.0f)
		dead();
}

// 床に当たった時の処理
void AdhereMiniBoss::floorHit(){}

// プレイヤーと当たった時の処理
void AdhereMiniBoss::playerHit(Actor & actor)
{
	if (state_ == State::Attack || state_ == State::Dead) return;
	changeState(State::Attack, ADHERE_NUMBER);
	animation_.setIsLoop(false);
	playerName_ = actor.getName();
	position_ = actor.getPosition();
	degree_ = getRandomDegree();
}

void AdhereMiniBoss::addAnimation()
{
	animation_.addAnimation(
		WAIT_NUMBER,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WAIT_TEX));
	animation_.addAnimation(
		RUN_NUMBER,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WALLATTACK_DASH_TEX));
	animation_.addAnimation(
		ADHERE_NUMBER,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_BREATH_DYSFUNCTION_TEX));
	animation_.addAnimation(
		DEAD_NUMBER,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_DEATH_TEX));
}

// ランダムの角度を取得します
float AdhereMiniBoss::getRandomDegree()
{
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	// 初期Seed値を渡す
	std::mt19937 mt(random());
	// 範囲の指定(int型)
	std::uniform_int_distribution<> degree(0, 360);
	return (float)degree(mt);
}
