#include "Stage3MiniBoss.h"
#include "../ImportAnimationNumber.h"
#include "../FlyingMiniBoss.h"
#include "../../Effect/PiyoriEffect.h"
#include "../../../FloorSearchPoint.h"
#include "../../../../../Base/ActorGroup.h"
#include "../../../../../../World/IWorld.h"
#include "../../../../../../Define.h"

Stage3MiniBoss::Stage3MiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const float bodyScale) : 
	MediumBoss(world, position, bodyScale),
	wallCount_(0),
	effectCreateTimer_(0.0f),
	groundTimer_(0.0f),
	mbTimer_(0.0f),
	isEffectCreate_(false),
	isPiyori_(false),
	isPrevWspHit_(false),
	prevAttackPosition_(position),
	wspObj_(nullptr)
{
	color_ = Vector3(240.0f, 80.0f, 0.0f);
	// 床捜索オブジェクト
	auto wspObj = std::make_shared<FloorSearchPoint>(
		world_, position_, Vector2::One * body_.GetCircle().getRadius(), 10.0f);
	world_->addActor(ActorGroup::Enemy, wspObj);
	wspObj_ = &*wspObj;

	// 方向コンテナに追加
	moveDirections_.clear();
	moveDirections_.push_back(0.0f);
	moveDirections_.push_back(-1.0f);
	moveDirections_.push_back(0.0f);
	moveDirections_.push_back(1.0f);
	moveDirections_.push_back(0.0f);
}

void Stage3MiniBoss::onUpdate(float deltaTime)
{
	MediumBoss::onUpdate(deltaTime);
	wspObj_->setPosition(position_);
}

void Stage3MiniBoss::battleIdel(float deltaTime)
{
	if (!isGround_)
		position_.y += 9.8f * (deltaTime * 60.0f);
	MediumBoss::battleIdel(deltaTime);
}

void Stage3MiniBoss::idel(float deltaTime)
{
	if (stateTimer_ < 2.0f) return;
	changeState(State::Attack, RUN_NUMBER);
	wspObj_->setDirection(Vector2::Left);
}

void Stage3MiniBoss::move(float deltaTime)
{
	if (isPiyori_)
		piyoriMove(deltaTime);
	else restMove(deltaTime);
}

void Stage3MiniBoss::attack(float deltaTime)
{
	// 壁移動攻撃
	wallAttack(deltaTime);
	// ミニボスの生成
	createMiniBoss(deltaTime);
}

void Stage3MiniBoss::deadMove(float deltaTime)
{
	// スターの削除
	if (stars_.size() != 0) {
		for (auto i = stars_.begin(); i != stars_.end(); i++) {
			auto star = *i;
			star->dead();
		}
		stars_.clear();
	}
	MediumBoss::deadMove(deltaTime);
}

void Stage3MiniBoss::piyoriMove(float deltaTime)
{
	auto dir = direction_;
	dir.y = 1.0f;
	animation_.setIsLoop(true);
	// 星の生成
	if (isEffectCreate_ && (int)effectCreateTimer_ % 50 <= 24 && stars_.size() < 6) {
		auto star = std::make_shared<PiyoriEffect>(
			world_, position_, 0.5f);
		world_->addActor(ActorGroup::Effect, star);
		stars_.push_back(star);
		isEffectCreate_ = false;
	}
	else if ((int)effectCreateTimer_ % 50 > 24) {
		isEffectCreate_ = true;
	}
	effectCreateTimer_ += deltaTime * 60.0f;
	// ぴよりエフェクトの位置
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto addPos = Vector2::Up * -50.0f;
		i->get()->position_ = position_ + addPos;
	}
	// 重力
	if (!isGround_) {
		position_.y += 9.8f * (deltaTime * 60.0f);
	}
	// 一定時間経過で待機状態に遷移
	if (stateTimer_ < 5.0f) return;
	changeState(State::Idel, WAIT_NUMBER);
	isPiyori_ = false;
	isEffectCreate_ = true;
	// スターの削除
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto star = *i;
		star->dead();
	}
	stars_.clear();
	effectCreateTimer_ = 0.0f;
}

// 休み行動
void Stage3MiniBoss::restMove(float deltaTime)
{
	if (stateTimer_ < 3.0f) return;
	changeState(State::Idel, WAIT_NUMBER);
}

void Stage3MiniBoss::playerAttackHit(Actor & actor)
{
	MediumBoss::playerAttackHit(actor);
	// 特定の位置に居る場合は、ぴより状態に遷移
	if (state_ == State::Attack) {
		if (wallCount_ % 4 != 0) {
			isPiyori_ = true;
			wallCount_ = 0;
			degree_ = 90.0f;
			changeState(State::Move, PIYORI_NUMBER);
		}
	}
}

// 壁移動攻撃処理
void Stage3MiniBoss::wallAttack(float deltaTime)
{
	// 時計周りで移動
	auto speed = 10.0f * (deltaTime * 60.0f);
	if (wspObj_->isGround() && isPrevWspHit_ != wspObj_->isGround()) wallCount_++;
	isPrevWspHit_ = wspObj_->isGround();
	auto direction = Vector2(
		moveDirections_[(wallCount_ + 1) % 4],
		moveDirections_[wallCount_ % 4]
		);
	direction_ = direction;
	wspObj_->setDirection(direction_);
	degree_ = (float)(wallCount_ * 90 % 360) + 90.0f;
	auto pos = direction * speed * (deltaTime * 60.0f);
	position_ += pos;
}

// 浮遊ミニボスの生成を行います
void Stage3MiniBoss::createMiniBoss(float deltaTime)
{
	mbTimer_ = max(mbTimer_ - deltaTime, 0.0f);
	if (mbTimer_ > 0.0f) return;
	mbTimer_ = getRandomInt(40, 100) / 10.0f;
	// 方向
	int chipsize = static_cast<int>(CHIPSIZE);
	auto pos = Vector2::Zero;
	auto count = getRandomInt(0, 2);
	if (count < 2) {
		// 左辺・右辺に生成
		if (count == 0) pos.x = chipsize * 18.0f;
		else if (count == 1) pos.x = chipsize * 2.0f;
		pos.y = (float)getRandomInt(chipsize * 2, chipsize * 7);
	}
	else if (count == 2) {
		// 上辺に生成
		pos.y = chipsize * 2.0f;
		pos.x = (float)getRandomInt(chipsize * 2, chipsize * 18);
	}
	// ミニボス(浮遊)の生成
	// 位置の補正
	auto position = position_;
	auto scrollPoint = world_->findActor("BossScrollPoint");
	if (scrollPoint != nullptr)
		position = scrollPoint->getPosition() -
		Vector2(chipsize * 10, chipsize * 6);
	auto miniBoss = std::make_shared<FlyingMiniBoss>(world_, pos + position);
	world_->addActor(ActorGroup::Enemy, miniBoss);
	PlaySoundMem(
		ResourceLoader::GetInstance().getSoundID(SoundID::SE_MINIBOSS_CRY),
		DX_PLAYTYPE_BACK);
}