#ifndef FLYING_MINI_BOSS_H_
#define FLYING_MINI_BOSS_H_

#include "AdhereMiniBoss.h"

class FlyingMiniBoss : public AdhereMiniBoss{
public:
	FlyingMiniBoss(
		IWorld* world, 
		const Vector2&  position, 
		const Vector2& direction = Vector2::One,
		const float bodyScale = 64.0f);

private:
	// 移動状態
	void move(float deltaTime) override;
	// 床に当たった時の処理
	void floorHit() override;
	// プレイヤーの攻撃に当たった時の処理
	void playerAttackHit(Actor& actor) override;

private:
	float deg_;				// 角度(親の変数名も変更する)
	Vector2 direction_;		// 方向
};

#endif
