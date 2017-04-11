#include "MiniBossManager.h"
#include "MiniBoss.h"

MiniBossManager::MiniBossManager()
{
}

void MiniBossManager::update(float deltaTime)
{
}

// ミニボスの追加を行います
void MiniBossManager::addMiniBoss(MiniBoss * miniBoss)
{
	miniBosses_.push_front(miniBoss);
}

// ミニボス全体の混乱状態を解除します
void MiniBossManager::backToSanity()
{
	for (auto i = miniBosses_.begin(); i != miniBosses_.end(); i++) {
		auto miniBoss = *i;
		miniBoss->backToSanity();
	}
}

// ミニボス全体がボスの位置にいるかを返します
bool MiniBossManager::isMiniBossAllBP()
{
	for (auto i = miniBosses_.begin(); i != miniBosses_.end(); i++) {
		auto miniBoss = *i;
		if (!miniBoss->isBossPosition()) 
			return false;
	}
	return true;
}

// ミニボスをボス持ち上げ状態にします
void MiniBossManager::bossLift()
{
	for (auto i = miniBosses_.begin(); i != miniBosses_.end(); i++) {
		auto miniBoss = *i;
		miniBoss->bossLift();
	}
}

// 持ち上げ終わったか
bool MiniBossManager::isLiftEnd()
{
	for (auto i = miniBosses_.begin(); i != miniBosses_.end(); i++) {
		auto miniBoss = *i;
		if (!miniBoss->isLift())
			return false;
	}
	return true;
}

// 持ち上げ移動速度の設定
void MiniBossManager::LiftMove(const float speed)
{
	for (auto i = miniBosses_.begin(); i != miniBosses_.end(); i++) {
		auto miniBoss = *i;
		miniBoss->setLiftMove(speed);
	}
}
