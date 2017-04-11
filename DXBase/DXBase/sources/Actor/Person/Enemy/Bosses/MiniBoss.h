#ifndef MINI_BOSS_H_
#define MINI_BOSS_H_

#include "../../../Base/Actor.h"
#include "../../../../Animation/Enemy/EnemyAnimation2D.h"

class FloorSearchPoint;

class MiniBoss : public Actor {
private:
	// アニメショーンクラス
	enum {
		MINI_BOSS_RUN = 0,
		MINI_BOSS_LIFT = 1,
	};
	// 状態クラス
	enum class State {
		Confusion,
		CalmDown,
		Approach,
		Lift,
		RunAway
	};

public:
	MiniBoss(IWorld* world, const Vector2& position, const float scale = 1.0f);
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;
	virtual void onMessage(EventMessage event, void*) override;

private:
	// 状態の更新
	void updateState(float deltaTime);
	// 状態の変更を行います
	void changeState(State state, int num);
	// 混乱状態
	void Confusion(float deltaTime);
	// 落ち着く状態
	void CalmDown(float deltaTime);
	// ボスに近づく状態
	void Approach(float deltaTime);
	// ボスを持ち上げる状態
	void Lift(float deltaTime);
	// 逃亡状態
	void RunAway(float deltaTime);
	// 地面の位置に補正します
	void groundClamp(Actor& actor);

public:
	//　ミニボスを正気に戻します
	void backToSanity();
	// ボスを持ち上げる状態にします
	void bossLift();
	// ボスの位置にいるかを返します
	bool isBossPosition();
	// 持ち上げたか
	bool isLift();
	// 
	void setLiftMove(const float speed);

private:
	float stateTimer_;
	float speed_;					// 移動速度
	float scale_;					// 画像の大きさ
	float bossPositionRX_;
	bool isConfusion_;				// 混乱しているか
	bool isBossPosition_;			// ボスの位置にいるか
	bool isLift_;
	Vector2 direction_;				// 方向
	State state_;					// 状態
	EnemyAnimation2D animation_;	// アニメーション
	FloorSearchPoint* fspObj_;		// 床捜索オブジェクト
};

#endif