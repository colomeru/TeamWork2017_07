#ifndef STAR_EFFECT_H_
#define STAR_EFFECT_H_

#include "../Base/Actor.h"
#include "../../Animation/Enemy/EnemyAnimation2D.h"

class StarEffect : public Actor{
private:
	enum class State {
		StartIdel,
		Idel,
		PositionMove,
		RotateMove,
		Imbide,
		Stop
	};

public:
	StarEffect(
		IWorld* world, 
		const Vector2& position,
		const Vector2& startPosition,
		const Vector2& movePosition,
		const Vector2& rotatePosition,
		const float timer);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	//void onCollide(Actor& actor) override;

private:
	void changeState(State state);
	void startIdel(const float deltaTime);
	void idel(const float deltaTime);
	void positionMove(float deltaTime);
	void rotateMove(float deltaTime);
	void imbide(float deltaTime);
	void stop();

public:
	void setIsNotIdel();
	void changeMove();
	void changeImbide();
	bool isIdelTimeEnd(const float time);
	bool isRotateMove();
	bool isRotateTimeEnd(const float time);
	bool isImbide();
	bool isStop();

private:
	float speed_;			// 速度
	float pMoveSpeed_;
	float degree_;
	float texDegree_;
	float idelTimer_;
	bool isIdel_;
	bool isStart_;
	Vector2 scale_;
	Vector2 startPosition_;
	Vector2 movePosition_;
	Vector2 rotatePosition_;

	float stateTimer_;		// 状態時間
	State state_;			// スターの状態
	ActorPtr player_;

	EnemyAnimation2D animation_;
};

#endif