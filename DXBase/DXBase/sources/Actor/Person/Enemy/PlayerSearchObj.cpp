#include "PlayerSearchObj.h"
#include "../../Base/ActorGroup.h"
#include "../../Body/CollisionBase.h"
#include "../../../World/IWorld.h"
#include "../../../Math/Math.h"

PlayerSearchObj::PlayerSearchObj(
	IWorld * world,
	const Vector2& enemyPosition,
	const Vector2& playerPosition) :
	Actor(world, "BaseEnemy",
		Vector2(
			std::abs(enemyPosition.x + playerPosition.x) / 2,
			std::abs(enemyPosition.y + playerPosition.y) / 2),
		CollisionBase(
			const_cast<Vector2&>(enemyPosition),
			const_cast<Vector2&>(playerPosition)
			)
		),
	enemyPosition_(enemyPosition),
	playerPosition_(playerPosition),
	blockPosition_(Vector2::Zero),
	isBlockHit_(false)
{
}

void PlayerSearchObj::onUpdate(float deltaTime)
{
	position_ = body_.setSegmentPoint(
		position_, enemyPosition_, playerPosition_);
	isBlockHit_ = false;
}

void PlayerSearchObj::onDraw() const{}

void PlayerSearchObj::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	// プレイヤー関連のオブジェクトに当たっているなら
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	// 床に当たっていたら、位置を代入
	if (getFloorName != NULL) {
		blockPosition_ = actor.position_;
		isBlockHit_ = true;
		return;
	}
}

// 敵とプレイヤーの位置を設定します
void PlayerSearchObj::setPosition(
	const Vector2 & enemyPosition, 
	const Vector2 & playerPosition)
{
	enemyPosition_ = enemyPosition;
	playerPosition_ = playerPosition;
}

// プレイヤーが見えているかを返します
bool PlayerSearchObj::isPlayerLook()
{
	return !isBlockHit_;
}
