#ifndef WALL_MOVE_ENEMY_H_
#define WALL_MOVE_ENEMY_H_

#include "../BaseEnemy.h"

// 壁移動エネミークラス
class WallMoveEnemy : public BaseEnemy {
public:
	WallMoveEnemy(
		IWorld * world,
		const Vector2& position,
		const Vector2& direction = Vector2(-1.0f, -1.0f));
	void onUpdate(float deltaTime) override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;
private:
	// 索敵移動です
	void search() override;
	// 索敵時の行動です
	void searchMove() override;
	// 壁捜索オブジェクトの位置を追加
	void addWSPPosition();
	// 壁捜索オブジェクトの大きさを追加
	void addWSPScale();
	// アニメーションの追加を行います
	void addAnimation() override;

private:
	Vector2 addScale_;	// 追加の大きさ
};

#endif
