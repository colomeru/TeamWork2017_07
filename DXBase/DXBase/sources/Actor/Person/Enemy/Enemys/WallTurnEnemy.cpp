#include "WallTurnEnemy.h"
#include "../EnemyHeaderImport.h"
#include "../../../Base/ActorGroup.h"
#include "../FloorSearchPoint.h"
#include "Prickle.h"

WallTrunEnemy::WallTrunEnemy(
	IWorld * world,
	const Vector2 & position,
	const Vector2& direction) :
	BaseEnemy(world, position, 64.0f, direction)
{
	speed_ *= 2;
	isUseGravity_ = false;
	isInvincible_ = true;
	// 壁捜索オブジェクトの生成
	auto wsObj = std::make_shared<FloorSearchPoint>(
		world_, position_,
		Vector2(
			(scale_ / 2.0f + 1.0f) * std::abs(direction_.x),
			(scale_ / 2.0f + 1.0f) * std::abs(direction_.y)),
		Vector2(
			scale_ * std::abs(direction_.y) + 2.0f,
			scale_ * std::abs(direction_.x) + 2.0f));
	// ワールドに追加
	world_->addActor(ActorGroup::Enemy, wsObj);
	wsObj_ = &*wsObj;
	objContainer_.push_back(wsObj_);
	// アニメーションの追加
	addTexPosition_ = Vector2::Zero;
	addAnimation();
	animation_.changeAnimation(ENEMY_WALK);
}

void WallTrunEnemy::update(float deltaTime)
{
	if (wsObj_->isGround())
		direction_ *= -1;
	if (direction_.y == -1)
		texDegress_ = 0;
	else texDegress_ = 180;
	wsObj_->setDirection(direction_);
	wsObj_->setPosition(position_);
}

void WallTrunEnemy::onMessage(EventMessage event, void *){}

void WallTrunEnemy::search()
{
	// プレイヤーの捜索
	findPlayer();
	// 捜索行動
	searchMove();
}

void WallTrunEnemy::attack(){}

void WallTrunEnemy::searchMove()
{
	position_ += direction_ * speed_ * deltaTimer_;
}

void WallTrunEnemy::chaseMove(){}

// アニメーションの追加を行います
void WallTrunEnemy::addAnimation()
{
	animation_.addAnimation(
		ENEMY_WALK,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::ENEMY_NEEDLEENEMY_TEX));
}
