#ifndef ADHERE_MINI_BOSS_H_
#define ADHERE_MINI_BOSS_H_

//#include "../../../../Base/Actor.h"
#include "FighterMiniBossh.h"
#include "../../../../../Animation/Enemy/EnemyAnimation2D.h"

class AdhereMiniBoss : public FighterMiniBoss {
protected:
	// 死亡状態
	enum class DeadState {
		HitDead,
		AdhereDead
	};

public:
	AdhereMiniBoss(
		IWorld* world, 
		const Vector2&  position, 
		const float bodyScale = 64.0f);

protected:
	// 状態の変更
	void changeDeadState(DeadState dState, int animation);
	// 待機状態
	void idel(float deltaTime) override;
	// 移動状態
	virtual void move(float deltaTime) override;
	// くっつき状態
	virtual void attack(float deltaTime) override;
	// 死亡状態
	virtual void deadMove(float deltaTime) override;
	// くっつき死亡状態
	void adhereDead(float deltaTime);
	// 床に当たった時の処理
	virtual void floorHit() override;
	// プレイヤーと当たった時の処理
	void playerHit(Actor& actor) override;

protected:
	//// アニメーションの追加
	virtual void addAnimation() override;
	// ランダムの角度を取得します
	float getRandomDegree();

protected:
	float adhereDeadTime_;	// くっつき時の死亡時間
	DeadState dState_;		// 死亡状態
};

#endif

