#include "WallMoveEnemy.h"
#include "../EnemyHeaderImport.h"
#include "../FloorSearchPoint.h"

WallMoveEnemy::WallMoveEnemy(
	IWorld * world,
	const Vector2 & position,
	const Vector2& direction) :
	BaseEnemy(world, position, 64.0f, direction),
	addScale_(4.0f, 4.0f)
{
	direction_ = direction;
	// 壁捜索オブジェクトの各ステータスの追加
	addWSPPosition();
	addWSPScale();
	// 壁捜索オブジェクトの追加
	createFSP();
	// 重力を使わない かつ 無敵
	isUseGravity_ = false;
	isInvincible_ = true;
	// アニメーションの追加
	addTexPosition_ = Vector2::Zero;
	addAnimation();
	animation_.changeAnimation(ENEMY_WALK);
}

void WallMoveEnemy::onUpdate(float deltaTime)
{
	// デルタタイムの値を設定する
	setDeltaTime(deltaTime);
	// エネミーマネージャーの更新
	enemyManager_.update(deltaTime);
	// アニメーションの更新
	animation_.update(deltaTime);
	// 状態の更新
	BaseEnemy::updateState(deltaTime);
	// 壁捜索オブジェクトの位置更新
	for (int i = 0; i != fspScaleContainer_.size(); i++) {
		enemyManager_.getWSPObj(i)->setPosition(position_);
	}
	isMoveFloor_ = false;
}

void WallMoveEnemy::onCollide(Actor & actor)
{
	BaseEnemy::onCollide(actor);
	// 壁移動の方向を設定
	enemyManager_.setIsDirection(isBlockCollideBegin_);
}

void WallMoveEnemy::onMessage(EventMessage event, void *){}

void WallMoveEnemy::search()
{
	// 状態遷移しない
	searchMove();
}

void WallMoveEnemy::searchMove()
{
	if (!isMoveFloor_) {
		// 方向を入れる
		direction_ = enemyManager_.getWallDirection();
		// 壁移動
		position_ += direction_ * speed_ * deltaTimer_;
		// アニメーションの再生
		// false => 反時計周り
		// 触れている床の位置で回す方向を変える
		if (enemyManager_.getWallAnimaDirection().x < 0 ||
			enemyManager_.getWallAnimaDirection().y < 0)
			animation_.setIsReverse(false);
		else animation_.setIsReverse(true);
	}
}

void WallMoveEnemy::addWSPPosition()
{
	// 位置の追加
	auto addPos = -1;
	// 0
	fspPositionContainer_.push_back(
		Vector2(-(scale_ / 2.0f + addScale_.x / 2.0f) - addPos,
			-(scale_ / 2.0f + addScale_.y / 2.0f) - addPos));
	// 1
	fspPositionContainer_.push_back(
		Vector2(0.0f, -(scale_ / 2.0f + addScale_.y / 2.0f) + -addPos));
	// 2
	fspPositionContainer_.push_back(
		Vector2(scale_ / 2.0f + addScale_.x / 2.0f + addPos,
			-(scale_ / 2.0f + addScale_.y / 2.0f) - addPos));
	// 3
	fspPositionContainer_.push_back(
		Vector2(-(scale_ / 2.0f + addScale_.x / 2.0f) + -addPos, 0.0f));
	// 4
	fspPositionContainer_.push_back(
		Vector2(scale_ / 2.0f + addScale_.x / 2.0f - -addPos, 0.0f));
	// 5
	fspPositionContainer_.push_back(
		Vector2(
			-(scale_ / 2.0f + addScale_.x / 2.0f) - addPos,
			scale_ / 2.0f + addScale_.y / 2.0f + addPos));
	// 6
	fspPositionContainer_.push_back(
		Vector2(0.0f, scale_ / 2.0f + addScale_.y / 2.0f - -addPos));
	// 7
	fspPositionContainer_.push_back(
		Vector2(scale_ / 2.0f + addScale_.x / 2.0f + addPos,
			scale_ / 2.0f + addScale_.y / 2.0f + addPos));
}

void WallMoveEnemy::addWSPScale()
{
	// 間隔を空ける
	auto addScale = 0.0f;
	// 0
	fspScaleContainer_.push_back(addScale_);
	// 1
	fspScaleContainer_.push_back(Vector2(scale_ +  addScale, addScale_.y));
	// 2
	fspScaleContainer_.push_back(addScale_);
	// 3
	fspScaleContainer_.push_back(Vector2(addScale_.x, scale_ + addScale));
	// 4
	fspScaleContainer_.push_back(Vector2(addScale_.x, scale_ + addScale));
	// 5
	fspScaleContainer_.push_back(addScale_);
	// 6
	fspScaleContainer_.push_back(Vector2(scale_ + addScale, addScale_.y));
	// 7
	fspScaleContainer_.push_back(addScale_);
}

// アニメーションの追加を行います
void WallMoveEnemy::addAnimation()
{
	animation_.addAnimation(
		ENEMY_WALK,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::ENEMY_WALLMOVEENEMY_TEX));
}
