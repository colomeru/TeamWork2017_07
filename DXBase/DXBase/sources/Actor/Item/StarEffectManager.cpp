#include "StarEffectManager.h"
#include "StarEffect.h"
#include "Effect/StarPowerUpEffect.h"
#include "../Base/ActorGroup.h"
#include "../../World/IWorld.h"
#include "../../Scene/Base/SceneDataKeeper.h"
#include "../../Define.h"

StarEffectManager::StarEffectManager() : 
	world_(nullptr),
	isPowerUpCreate_(false),
	isEffectEnd_(true)
{
}

StarEffectManager::StarEffectManager(IWorld* world) :
	world_(world),
	isPowerUpCreate_(false),
	isEffectEnd_(false)
{
	initialeze();
}

void StarEffectManager::initialeze()
{
	// 星関連
	startPositiones_.clear();
	stars_.clear();
	//starCount_ = (int)(keeper_->GetItemCount("All") / 100);
	starCount_ = (int)(world_->GetKeeper()->GetItemCount("All") / 100);
	starCount_ = (int)MathHelper::Clamp((float)starCount_, 1.0f, 5.0f);
	isCreateStar_ = false;
	isIdelEnd_ = false;
	isEffectEnd_ = false;
}

void StarEffectManager::update()
{
	// 演出が終了したら、返す
	if (isEffectEnd_) return;
	// 星の生成
	if (!isCreateStar_) {
		createStars();
	}
	// 星の更新
	updateStars();
}

void StarEffectManager::end()
{
	world_ = nullptr;
}

// 星の位置の設定
void StarEffectManager::setStartPosition(
	const Vector2 & position, 
	const float addPositionX)
{
	// 1
	startPositiones_.push_back(position);
	// 2
	startPositiones_.push_back(position + Vector2::Right * addPositionX);
	// 3
	startPositiones_.push_back(position + Vector2::Left * addPositionX);
	// 4
	startPositiones_.push_back(position + Vector2::Right * addPositionX);
	// 5
	startPositiones_.push_back(position + Vector2::Left * addPositionX);
}

// 星の演出が終了したかを返します
bool StarEffectManager::isEffectEnd()
{
	return isEffectEnd_;
}

// 星の生成
void StarEffectManager::createStars()
{
	/*auto pos = Vector2(CHIPSIZE * 9, 500.0f);
	setStartPosition(pos);*/
	auto players = world_->findActor("Player");
	auto startPos = Vector2::Zero;
	auto movePos = Vector2::Zero;
	auto rotatePos = Vector2::Zero;
	if (players != nullptr) {
		movePos = players->getPosition() - Vector2(-5.0f, 140.0f);
		rotatePos = players->getPosition();
	}
	for (auto i = 0; i != starCount_; i++) {
		auto star = std::make_shared<StarEffect>(
			world_, startPositiones_[i] - Vector2::Up * 500.0f -
			(Vector2::Up * 100.0f * i),
			startPositiones_[i],
			movePos, rotatePos, i * 0.83f / starCount_);
		world_->addActor(ActorGroup::Effect, star);
		stars_.push_front(star.get());
	}
	isCreateStar_ = true;
}

// 星の更新
void StarEffectManager::updateStars()
{
	// isEffectEnd_ = true;
	if (!isPowerUpCreate_) moveStars();
	else {
		auto effect = world_->findActor("StarPowerUpEffect");
		if (effect == nullptr) isEffectEnd_ = true;
	}
}

// 星を動かします
void StarEffectManager::moveStars()
{
	// 待機状態が終わっていない場合
	if (!isIdelEnd_) {
		auto isIdelEnd = true;
		// 星を確かめる
		for (auto j = stars_.begin(); j != stars_.end(); j++) {
			auto star = *j;
			if (!star->isIdelTimeEnd(2.0f)) {
				isIdelEnd = false;
			}
		}
		// 待機状態で、一定時間経過したら、指定の位置に移動
		if (isIdelEnd)
			isIdelEnd_ = true;
	}
	else {
		auto isRotateEnd = true;
		auto isStop = true;
		auto count = 0.0f;
		for (auto k = stars_.begin(); k != stars_.end(); k++) {
			auto star = *k;
			star->setIsNotIdel();
			// 一定時間回転したら、吸収状態に遷移させる
			if (star->isRotateTimeEnd(2.0f + count)) {
				star->changeImbide();
			}
			count += 0.5f;
			if (!star->isStop())
				isStop = false;
		}
		// 全員止まっていたら、終了処理を行う
		if (isStop) {
			// 削除する
			for (auto i = stars_.begin(); i != stars_.end(); i++) {
				auto star = *i;
				star->dead();
			}
			stars_.clear();
			// パワーアップエフェクトの生成
			auto players = world_->findActor("Player");
			auto movePos = Vector2::Zero;
			if (players != nullptr)
				movePos = players->getPosition();// -Vector2(-5.0f, 140.0f);
			auto effect = std::make_shared<StarPowerUpEffect>(world_, movePos);
			world_->addActor(ActorGroup::Effect, effect);
			isPowerUpCreate_ = true;
		}
	}
}
