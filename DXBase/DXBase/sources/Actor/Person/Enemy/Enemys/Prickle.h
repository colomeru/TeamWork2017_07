#ifndef PRICKLE_H_
#define PRICKLE_H_

#include "../../../Base/Actor.h"
#include "../../../../Animation/Base/Animation2D.h"

// トゲクラス (エネミーの所持している武器)
class Prickle : public Actor {
public:
	Prickle(
		IWorld* world,
		const Vector2& enemyPosition
		, const Vector2& addPosition,
		const Vector2& bodyScale);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;
	// 位置を入れます
	void setEnemyPosition(const Vector2& position);
	// 追加位置を入れます
	void setAddPosition(const Vector2& position);
	// 方向を入れます
	void setDirection(const Vector2& direction);
	// プレイヤーに与えるダメージ量を返します
	int getAP();

private:
	int ap_;				// プレイヤーに与えるダメージ量
	Vector2 direction_;		// 方向
	Vector2 enemyPosition_;	// 敵の位置
	Vector2 addPosition_;	// 追加の位置
};

#endif
