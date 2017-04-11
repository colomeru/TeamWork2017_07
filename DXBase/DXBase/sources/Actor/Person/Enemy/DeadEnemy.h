#ifndef DEADENEMY_H_
#define DEADENEMY_H_

#include "../../Base/Actor.h"
#include "../../../Animation/Enemy/EnemyAnimation2D.h"
#include "../../../ResourceLoader/ResourceLoader.h"

// 死亡エネミー(衝突判定のみ)
class DeadEnemy : public Actor {
private:
	enum class State {
		Dead,
		Hold,
		Throw,
		Delete,
	};

public:
	DeadEnemy(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale,
		const Vector2& direction = Vector2(1.0f, 1.0f),
		const AnimationID animaID = AnimationID::ENEMY_EGGENEMY_DAMAGE_TEX);
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;

private:
	// 状態の更新
	void updateState(float deltaTime);
	// 状態の変更
	void changeState(State state);
	// 死亡状態
	void deadMove();
	// 持ち上げ状態
	void holdMove(float deltaTime);
	// 投げ状態
	void throwMove(float deltaTime);
	// 消滅状態
	void deleteMove(float deltaTime);
	// 衝突判定の更新
	void updateCollide();
	// 地面の位置に補正します
	void groundClamp(Actor& actor);
	// プレイヤーの名前を設定します
	void setPlayerName();

public:
	// 色の変更を行います
	void setColor(Vector3 color);

private:
	float stateTimer_;				// 状態タイマ
	bool isGround_;					// 接地しているか
	EnemyAnimation2D animation_;	// アニメーション
	State state_;					// 状態

	int starCount_;
	float playerSpeed_;				// プレイヤーの移動速度
	float holdLength_;				// つかまれた位置との距離
	float prevHoldDegree_;			// つかまれた時の前回の角度
	float throwDegree_;				// 投げられた時の角度
	Vector2 playerPravPosition_;	// プレイヤーの前回の位置
	Vector2 holdPosition_;			// つかまれた位置
	Vector2 prevPosition_;			// 前回の位置
	Vector2 throwVector_;			// 投げられたときのベクトル
	Vector3 color_;					// 色
	bool isHold_;					// つかまっているか
	std::string playerName_;		// つかんだ相手の名前
	std::string otherName_;			// もう片方の名前

private:
	bool isBlockCollideBegin_;		// ブロックと当たっているか(初回時)
	bool isBlockCollideEnter_;		// ブロックと当たっているか(衝突中)
	bool isBlockCollidePrevEnter_;	// ブロックと当たっているか(過去の衝突中)
	bool isBlockCollideExit_;		// ブロックと当たっているか(衝突後)
};

#endif
