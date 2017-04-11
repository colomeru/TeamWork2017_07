#include "MiniBoss.h"
#include "../../../Base/ActorGroup.h"
#include "../../../../ResourceLoader/ResourceLoader.h"
#include "../FloorSearchPoint.h"
#include "../../../../World/IWorld.h"
#include "../../../../Math/Math.h"
#include <random>

MiniBoss::MiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const float scale) : 
	Actor(world, "MiniRunBoss", position,
		CollisionBase(const_cast<Vector2&>(position), 64.0f * scale)),
	stateTimer_(0.0f),
	speed_(6.0f),
	scale_(scale),
	bossPositionRX_(0.0f),
	isConfusion_(true),
	isBossPosition_(false),
	isLift_(false),
	direction_(Vector2::One * -1),
	state_(State::Confusion),
	animation_(EnemyAnimation2D()),
	fspObj_(nullptr)
{
	// 方向を変える
	auto size = (int)(scale * 100);
	if (size % 2 == 1) direction_.x = 1;
	// アニメーションの追加
	animation_.addAnimation(MINI_BOSS_RUN,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WALLATTACK_DASH_TEX));
	animation_.addAnimation(MINI_BOSS_LIFT,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_DAMAGE_TEX));
	animation_.changeAnimation(MINI_BOSS_RUN);
	animation_.changeDirType(direction_.x);
	// 床捜索オブジェクト
	auto fspObj = std::make_shared<FloorSearchPoint>(
		world_, position_,
		Vector2(body_.GetCircle().getRadius(), body_.GetCircle().getRadius()), 
		10.0f);
	world_->addActor(ActorGroup::EnemyBullet, fspObj);
	fspObj_ = fspObj.get();
	fspObj_->setDirection(direction_);
}

void MiniBoss::onUpdate(float deltaTime)
{
	// 状態の更新
	updateState(deltaTime);
	animation_.update(deltaTime);
}

void MiniBoss::onDraw() const
{
	auto size = 256;
	animation_.draw(
		position_, 
		Vector2(size / 2.0f, size / 1.5f),
		scale_);
}

void MiniBoss::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	//床関連のオブジェクトに当たっているなら
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	if (actorName == "Door") {
		// ドアを壊す
		if (state_ == State::RunAway) {
			actor.dead();
			auto se =
				ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_CHAKUCHI);
			if (CheckSoundMem(se) == 0)
				PlaySoundMem(se, DX_PLAYTYPE_BACK);
			return;
		}
		else if (state_ == State::Approach) {
			// 目的の位置にたどり着けない場合は、到達したことにする。
			groundClamp(actor);
			/*if (std::abs(position_.x - bossPositionRX_) < 100.0f) {
				isBossPosition_ = true;
				animation_.setIsLoop(false);
				animation_.stopAnime();
			}*/
			return;
		}
	}
	// マップのブロックに当たったら、処理を行う
	if (getFloorName != NULL || actorName == "Door") {
		// 位置の補間
		groundClamp(actor);
		return;
	}
}

void MiniBoss::onMessage(EventMessage event, void *)
{
}

// 状態の更新
void MiniBoss::updateState(float deltaTime)
{
	// 状態の更新
	switch (state_)
	{
	case State::Confusion: Confusion(deltaTime); break;
	case State::CalmDown: CalmDown(deltaTime); break;
	case State::Approach: Approach(deltaTime); break;
	case State::Lift: Lift(deltaTime); break;
	case State::RunAway: RunAway(deltaTime); break;
	}

	stateTimer_ += deltaTime;
}

// 状態の変更を行います
void MiniBoss::changeState(State state, int num)
{
	if (state_ == state) return;
	stateTimer_ = 0.0f;
	state_ = state;
	animation_.changeAnimation(num);
	animation_.setIsLoop(true);
}

// 混乱状態
void MiniBoss::Confusion(float deltaTime)
{
	position_.y += 9.8f * (deltaTime * 60.0f);
	position_.x += direction_.x * speed_ * (deltaTime * 60.0f);
	fspObj_->setPosition(position_);
	// 方向を変える
	if (fspObj_->isGround()) {
		direction_.x *= -1;
		animation_.changeDirType(direction_.x);
	}
	fspObj_->setDirection(direction_);

	// 混乱状態でなければ、落ち着き状態に遷移
	if (!isConfusion_) {
		//changeState(State::CalmDown, MINI_BOSS_RUN);
		auto boss = world_->findActor("DeadBoss");
		if (boss == nullptr) {
			changeState(State::Approach, MINI_BOSS_RUN);
			return;
		}
		auto posX = (int)boss->position().x;
		// 乱数の取得
		std::random_device random;
		// メルセンヌツイスター法 後で調べる
		// 初期Seed値を渡す
		std::mt19937 mt(random());
		// 範囲の指定(int型)
		std::uniform_int_distribution<> bpos(-100, 100);
		bossPositionRX_ = (float)(posX + bpos(mt));
		changeState(State::Approach, MINI_BOSS_RUN);
	}
}

// 落ち着く状態
void MiniBoss::CalmDown(float deltaTime)
{
	animation_.setIsLoop(false);
	if (stateTimer_ >= 2.0f) {
		auto boss = world_->findActor("DeadBoss");
		if (boss == nullptr) {
			changeState(State::Approach, MINI_BOSS_RUN);
			return;
		}
		auto posX = (int)boss->position().x;
		// 乱数の取得
		std::random_device random;
		// メルセンヌツイスター法 後で調べる
		// 初期Seed値を渡す
		std::mt19937 mt(random());
		// 範囲の指定(int型)
		std::uniform_int_distribution<> bpos(-100, 100);
		bossPositionRX_ = (float)(posX + bpos(mt));
		changeState(State::Approach, MINI_BOSS_RUN);
	}
}

// ボスに近づく状態
void MiniBoss::Approach(float deltaTime)
{
	if (isBossPosition_) return;
	//auto boss = world_->findActor("DeadBoss");
	//if (boss == nullptr) return;
	auto distance = position_.x - bossPositionRX_;
	// 移動
	if (distance < 0) direction_.x = 1.0f;
	else direction_.x = -1.0f;
	animation_.changeDirType(direction_.x);
	//auto speed = speed_;
	//if (distance < speed) speed = std::abs(distance);
	position_.x += direction_.x * speed_ * (deltaTime * 60.0f);
	if (std::abs(distance) < speed_) {
		isBossPosition_ = true;
		animation_.setIsLoop(false);
		animation_.stopAnime();
	}
}

// ボスを持ち上げる状態
void MiniBoss::Lift(float deltaTime)
{
	animation_.setIsLoop(false);
	animation_.changeDirType(-1.0f);
	if (stateTimer_ >= 2.0f) isLift_ = true;
}

// 逃亡状態
void MiniBoss::RunAway(float deltaTime)
{
	direction_.x = -1.0f;
	position_.x += direction_.x * speed_ * (deltaTime * 60.0f);
}

//地面の位置に補正します
void MiniBoss::groundClamp(Actor& actor)
{
	if (actor.body_.GetBox().getWidth() == 0) return;
	// 新円と正方形の衝突判定
	// 自分自身の1f前の中心位置を取得
	auto isHit = false;
	auto pos = body_.GetCircle().previousPosition_;
	// 相手側の四角形の4点を取得
	auto topLeft = actor.getBody().GetBox().component_.point[0];
	auto topRight = actor.getBody().GetBox().component_.point[1];
	auto bottomLeft = actor.getBody().GetBox().component_.point[2];
	auto bottomRight = actor.getBody().GetBox().component_.point[3];
	// 外積を使って、縦の長さを計算する
	auto top = Vector2::Cross(
		(topLeft - topRight).Normalize(), (pos - topRight));
	auto bottom = Vector2::Cross(
		(bottomRight - bottomLeft).Normalize(), (pos - bottomLeft));
	auto right = Vector2::Cross(
		(topRight - bottomRight).Normalize(), (pos - bottomRight));
	auto left = Vector2::Cross(
		(bottomLeft - topLeft).Normalize(), (pos - topLeft));
	// 過去の位置
	// Y方向に位置を補間する
	if (left < 0 &&
		right < 0) {
		// 上に補間
		if (top > -actor.getBody().GetBox().getHeight() / 3) {
			position_.y = topLeft.y - body_.GetCircle().getRadius();
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 3) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
		}
	}
	// X方向に位置を補間する
	else if (top < 0 &&
		bottom < 0) {
		// 左に補間
		if (left > 0) {
			position_.x = bottomLeft.x - body_.GetCircle().getRadius();
			isHit = true;
		}
		// 右に補間
		if (right > 0) {
			position_.x = topRight.x + body_.GetCircle().getRadius();
			isHit = true;
		}
		if (state_ == State::Approach) {
			if (std::abs(position_.x - bossPositionRX_) < 100.0f) {
				isBossPosition_ = true;
				animation_.setIsLoop(false);
				animation_.stopAnime();
			}
		}
	}

	if (isHit) return;

	// 現在の判定
	pos = position_;
	// 外積を使って、縦の長さを計算する
	top = Vector2::Cross(
		(topLeft - topRight).Normalize(), (pos - topRight));
	bottom = Vector2::Cross(
		(bottomRight - bottomLeft).Normalize(), (pos - bottomLeft));
	right = Vector2::Cross(
		(topRight - bottomRight).Normalize(), (pos - bottomRight));
	left = Vector2::Cross(
		(bottomLeft - topLeft).Normalize(), (pos - topLeft));
	// Y方向に位置を補間する
	if (left < 0 &&
		right < 0) {
		// 上に補間
		if (top > -actor.getBody().GetBox().getHeight() / 2.0f) {
			position_.y = topLeft.y - body_.GetCircle().getRadius();
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 2.0f) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
		}
	}
	// X方向に位置を補間する
	if (top < 0 &&
		bottom < 0) {
		// 左に補間
		if (left > -actor.getBody().GetBox().getWidth() / 2.0f)
			position_.x = bottomLeft.x - body_.GetCircle().getRadius();
		// 右に補間
		if (right > -actor.getBody().GetBox().getWidth() / 2.0f)
			position_.x = topRight.x + body_.GetCircle().getRadius();
		if (state_ == State::Approach) {
			if (std::abs(position_.x - bossPositionRX_) < 100.0f) {
				isBossPosition_ = true;
				animation_.setIsLoop(false);
				animation_.stopAnime();
			}
		}
	}
}

//　ミニボスを正気に戻します
void MiniBoss::backToSanity()
{
	isConfusion_ = false;
}

// ボスを持ち上げる状態にします
void MiniBoss::bossLift()
{
	changeState(State::Lift, MINI_BOSS_LIFT);
}

// ボスの位置にいるかを返します
bool MiniBoss::isBossPosition()
{
	return isBossPosition_;
}

// 持ち上げたか
bool MiniBoss::isLift()
{
	return isLift_;
}

void MiniBoss::setLiftMove(const float speed)
{
	speed_ = speed;
	changeState(State::RunAway, MINI_BOSS_RUN);
}
