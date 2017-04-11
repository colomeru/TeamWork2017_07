#pragma once
#include "../Base/Actor.h"
#include "../../World/IWorld.h"
// プレーヤー
class BlockParticle : public Actor {
public:
	BlockParticle(IWorld* world, const Vector2& position);
	~BlockParticle();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other) override;
public:
	//壊すかどうか
	void Break(bool flag);
	//直されたかどうか
	void RepairBlock(bool flag);
	Vector2 GetFirstPosition();
private:
	//初期位置
	Vector2 mPosition;
	Vector2 mVelo;
	bool mBreakFlag;
	bool mRepairFlag;
	float mAngle;
	float mAngleVelo;
	//描写の座標
	Vector2 mDrawPos;
};