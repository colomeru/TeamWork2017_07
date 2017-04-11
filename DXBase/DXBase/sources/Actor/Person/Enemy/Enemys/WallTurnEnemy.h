#ifndef WALL_TRUN_ENEMY_H
#define WALL_TRUN_ENEMY_H

#include "../BaseEnemy.h"

class FloorSearchPoint;

// ゴルドエネミークラス
class WallTrunEnemy : public BaseEnemy {
public:
	WallTrunEnemy(
		IWorld * world, 
		const Vector2& position,
		const Vector2& direction = Vector2(0.0f, -1.0f));
	void update(float deltaTime) override;
	void onMessage(EventMessage event, void*) override;

private:
	// 索敵移動です
	void search() override;
	// 攻撃行動です
	void attack() override;
	// 索敵時の行動です
	void searchMove() override;
	// 追跡時の行動です
	void chaseMove() override;
	// アニメーションの追加を行います
	void addAnimation() override;

private:
	FloorSearchPoint* wsObj_;
};

#endif
