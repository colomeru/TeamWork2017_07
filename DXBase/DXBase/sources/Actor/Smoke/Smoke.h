#pragma once
#include "../Base/Actor.h"
#include "../../World/IWorld.h"
#include "../../Animation/Base/Animation2D.h"
#include <list>
// プレーヤー
class Smoke : public Actor {
public:
	struct AnimState
	{
		Animation2D anim = Animation2D();
		Vector2 position=Vector2::Zero;
		float alpha=0.0f;
		float scale=0.0f;
		float angle = 0.0f;
	};
public:
	Smoke(IWorld* world, const Vector2& position);
	~Smoke();
	//削除用関数
	bool RemoveState(const AnimState& anim);
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other) override;
public:
	//煙をたくか
	void SmokeFlag(bool flag);

private:
	//煙の時間
	float mSmokeTime;
	bool mIsSmoke;
	Vector2 mAnimSize;
	std::list<AnimState> mSmokeAnims;
};