#ifndef PLAYER_SEARCH_OBJ_H_
#define PLAYER_SEARCH_OBJ_H_

#include "../../Base/Actor.h"

class PlayerSearchObj : public Actor {
public:
	PlayerSearchObj(
		IWorld* world,
		const Vector2& enemyPosition,
		const Vector2& playerPosition);
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;
	// 敵とプレイヤーの位置を設定します
	void setPosition(const Vector2& enemyPosition, const Vector2& playerPosition);
	// プレイヤーが見えているかを返します
	bool isPlayerLook();

private:
	Vector2 enemyPosition_;		// 敵の位置
	Vector2 playerPosition_;	// プレイヤーの位置
	Vector2 blockPosition_;		// ブロックの位置
	bool isBlockHit_;
};

#endif