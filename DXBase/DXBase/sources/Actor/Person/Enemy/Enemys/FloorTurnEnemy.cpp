#include "FloorTurnEnemy.h"
#include "../../../Base/ActorGroup.h"
#include "../FloorSearchPoint.h"
#include "Prickle.h"
#include "../EnemyHeaderImport.h"

#include "../Bosses/Effect/EnemyDeadEffect.h"

FloorTurnEnemy::FloorTurnEnemy(
	IWorld * world, 
	const Vector2 & position,
	float right) :
	BaseEnemy(world, position, 64.0f),
	fspObj_(nullptr)
{
	direction_ = Vector2(-right, 1.0f);
	BaseEnemy::Initialize();
	// 崖捜索オブジェクトの追加
	auto fsObj = std::make_shared<FloorSearchPoint>(
		world_, 
		position_,
		Vector2((scale_ / 2.0f + 1.0f), scale_ / 2.0f), 
		Vector2(30.0f, 30.0f));
	world_->addActor(ActorGroup::Enemy, fsObj);
	fspObj_ = &*fsObj;
	objContainer_.push_back(fspObj_);
	// トゲオブジェクトの生成
	auto pricleObj = std::make_shared<Prickle>(
		world_,
		position_,
		Vector2::Up * -(scale_+ 1.0f), 
		Vector2(64.0f, 64.0f));
	world_->addActor(ActorGroup::Enemy_AttackRange, pricleObj);
	pricleObj_ = &*pricleObj;
	objContainer_.push_back(pricleObj_);
}

void FloorTurnEnemy::beginUpdate(float deltaTime){}

void FloorTurnEnemy::update(float deltaTime)
{
	// 床捜索オブジェクトが何も触れていなくて、接地している場合
	// 追跡状態の場合は行わない
	if (!fspObj_->isGround() && isGround_ && state_ != State::Chase) {
		direction_.x *= -1;
	}
	// 崖捜索オブジェクトの更新
	fspObj_->setDirection(direction_);
	fspObj_->setPosition(position_);
	// トゲの更新
	pricleObj_->setDirection(direction_);
	pricleObj_->setEnemyPosition(position_);
}

void FloorTurnEnemy::onMessage(EventMessage event, void *){}

// 所持しているオブジェクトの位置を設定します
void FloorTurnEnemy::setObjPosition()
{
	BaseEnemy::setObjPosition();
	fspObj_->setPosition(position_);
	pricleObj_->setEnemyPosition(position_);
}

void FloorTurnEnemy::attack(){}

void FloorTurnEnemy::searchMove()
{
	// 崖移動
	position_ += direction_.x * speed_ * deltaTimer_;
	pricleObj_->setAddPosition(Vector2::Up * -(scale_ + 1.0f));
}

void FloorTurnEnemy::chaseMove()
{
	// 通常時よりも速めにする
	auto speed = speed_;
	auto distance = position_.x - enemyManager_.getPlayerPosition(discoveryNumber_).x;
	//player_->getPosition().x;
	//// プレイヤーの軸付近に来たら止まる
	if (std::abs(distance) < speed) return;
	// プレイヤーとの位置で、速度を補正する
	if (std::abs(distance) < speed)
		speed = std::abs(distance);
	// 追跡
	// プレイヤーとのX軸が同一の場合おかしくなる
	direction_.x = -enemyManager_.getPlayerDirection(discoveryNumber_).x;
	position_.x += enemyManager_.getPlayerDirection(discoveryNumber_).x *
		-speed * deltaTimer_;
	pricleObj_->setAddPosition(Vector2((scale_ + 1.0f), 0.0f));
}
