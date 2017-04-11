#ifndef BOSS_WING_EFFECT_H_
#define BOSS_WING_EFFECT_H_

#include "../../../../Base/Actor.h"
#include <random>

class BossWingEffect : public Actor {
private:
	enum class State {
		FlyOut,
		Fall,
		Dead
	};

public:
	BossWingEffect(
		IWorld* world, 
		const Vector2& position, 
		const float bodyScale = 16.0f);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;

private:
	// 状態の変更
	void changeState(State state);
	// 排出状態
	void flyOut(float deltaTime);
	// 落下状態
	void fall(float deltaTime);
	// 死亡状態
	void deadMove(float deltaTime);
	// ベジェ曲線を取得します(始点・終点0)
	Vector2 bezier(float deltaTime, const Vector2& direPoint);
	// ベジェ曲線を取得します(始点と終点の指定)
	Vector2 bezier(
		float deltaTime, const Vector2& startPoint,
		const Vector2& direPoint, const Vector2& endPoint);
	void groundClamp(Actor& actor);
	// ランダムの値を取得します
	int getRandomInt(const int min, const int max);

private:
	//float scale_;		// 大きさ
	float rotaSpeed_;	// テクスチャの回転速度
	float degree_;		// 排出時の角度
	float texDegree_;	// テクスチャの角度
	float stateTimer_;	// 状態タイマ
	float bezierTimer_;	// ベジェタイマ
	float flyPower_;	// 排出時の力
	//float alpha_;		// テクスチャの透明度
	bool isTexTurn_;	// テクスチャを反転するか
	Vector2 direction_;	// 方向
	Vector3 color_;
	State state_;		// 状態
	std::mt19937 mt_;	// 乱数の初期seed
};

#endif
