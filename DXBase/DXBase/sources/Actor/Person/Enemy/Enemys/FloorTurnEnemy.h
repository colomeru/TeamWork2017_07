#ifndef FLOOR_TURN_ENEMY_H_
#define FLOOR_TURN_ENEMY_H_

#include "../BaseEnemy.h"

class FloorSearchPoint;

// クリボーエネミークラス
class FloorTurnEnemy : public BaseEnemy {
public:
	FloorTurnEnemy(
		IWorld * world, 
		const Vector2& position,
		float right = 1.0f);
	void beginUpdate(float deltaTime) override;
	void update(float deltaTime) override;
	void onMessage(EventMessage event, void*) override;
	// 所持しているオブジェクトの位置を設定します
	void setObjPosition() override;

private:
	// 攻撃行動です
	void attack() override;
	// 索敵時の行動です
	void searchMove() override;
	// 追跡時の行動です
	void chaseMove() override;

private:
	FloorSearchPoint* fspObj_;
};

#endif