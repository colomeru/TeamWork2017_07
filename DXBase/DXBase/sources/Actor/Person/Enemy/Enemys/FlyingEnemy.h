#ifndef FLYING_ENEMY_H_
#define FLYING_ENEMY_H_

#include "../BaseEnemy.h"

class FloorSearchPoint;

// ハネクリボーエネミークラス
class FlyingEnemy : public BaseEnemy {
public:
	FlyingEnemy(
		IWorld * world,
		const Vector2& position,
		float Down = 1.0f);
	void beginUpdate(float deltaTime) override;
	void update(float deltaTime) override;
	void onMessage(EventMessage event, void*) override;

private:
	// 索敵移動です
	void search() override;
	// プレイヤーを発見した時の行動です
	void discovery() override;
	// 攻撃行動です
	void attack() override;
	// 索敵時の行動です
	void searchMove() override;
	// プレイヤーの追跡行動です
	void chase() override;
	// 追跡時の行動です
	void chaseMove() override;
	// 敵を見失った時の行動です
	void lostMove() override;
	// アニメーションの追加を行います
	void addAnimation() override;

private:
	float direTimer_;			// 方向転換する時間
	float lostTimer_;			// 見失った時間
	Vector2 pastPosition_;
	FloorSearchPoint* wsObj_;	// 壁捜索オブジェクト
};

#endif
