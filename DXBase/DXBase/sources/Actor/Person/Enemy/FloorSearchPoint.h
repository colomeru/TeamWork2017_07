#ifndef FLOOR_SEARCH_POINT_H_
#define FLOOR_SEARCH_POINT_H_

#include "../../Base/Actor.h"

class FloorSearchPoint : public Actor {
public:
	// コンストラクタ(矩形)
	FloorSearchPoint(
		IWorld* world, 
		const Vector2& enemyPosition,
		const Vector2& addPosition, 
		const Vector2& bodyScale
		);
	// コンストラクタ(円)
	FloorSearchPoint(
		IWorld* world,
		const Vector2& enemyPosition,
		const Vector2& addPosition,
		const float radius
		);
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;
	virtual void onMessage(EventMessage event, void*) override;
	// 位置の設定
	void setPosition(const Vector2& position);
	// 方向の設定
	void setDirection(const Vector2& direction);
	// 床と当たったかを返します
	bool isFloor();
	bool isGround();
	// 床と当たった場所を返します
	Vector2 getFloorPosition();

private:
	int turnCount_;			// 振り向き回数
	bool isFloor_;
	bool isGround_;
	bool isGroundBegin_;
	Vector2 direction_;		// 方向
	Vector2 enemyPosition_;
	Vector2 addPosition_;
	Vector2 floorPosition_;	// 床の位置
};

#endif