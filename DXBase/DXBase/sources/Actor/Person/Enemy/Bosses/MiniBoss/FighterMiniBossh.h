#ifndef FIGHTER_MINI_BOSS_H_
#define FIGHTER_MINI_BOSS_H_

#include "../../../../Base/Actor.h"
#include "../../../../../Animation/Enemy/EnemyAnimation2D.h"

class FighterMiniBoss : public Actor {
protected:
	enum class State {
		BattleIdel,
		Idel,
		Move,
		Attack,
		Dead
	};

public:
	FighterMiniBoss(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale = 64.0f,
		const std::string name = "MiniBoss");
	virtual void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;

protected:
	// 状態の変更
	void changeState(State state, int animation);
	// 状態の更新
	void updateState(float deltaTime);
	// 勝負待機状態
	virtual void battleIdel(float deltaTime);
	// 待機状態
	virtual void idel(float deltaTime);
	// 移動状態
	virtual void move(float deltaTime);
	// 攻撃状態
	virtual void attack(float deltaTime);
	// 死亡状態
	virtual void deadMove(float deltaTime);
	// 床に当たった時の処理
	virtual void floorHit();
	// プレイヤーに当たった時の処理	
	virtual void playerHit(Actor& actor);
	// プレイヤーの攻撃に当たった時の処理
	virtual void playerAttackHit(Actor& actor);

protected:
	// プレイヤーとの方向を返します(単位ベクトル)
	Vector2 getPlayerDirection();
	// アニメーションの追加
	virtual void addAnimation();
	// ボスのアルファ値を設定します
	void texAlpha(float deltaTime);
	// 地面の位置に補正します
	void groundClamp(Actor& actor);

protected:
	int animeNum_;					// アニメーション番号
	float speed_;					// 速度
	float stateTimer_;				// 時間
	float size_;					// 大きさ
	float degree_;					// 角度
	float damegeTimer_;
	//float adhereDeadTime_;			// くっつき時の死亡時間
	bool isClamp_;					// 地面の位置に補間するか
	bool isGround_, isBottom_;		// 床の上下に当たったか
	bool isLeft_, isRight_;			// 床の左右に当たったか
	bool isAttackHit_;				// 攻撃に当たったか
	bool isInvincible_;				// 無敵か
	std::string playerName_;		// プレイヤーの名前
	Vector2 addTexPos_;				// テクスチャの位置
	Vector2 orizin_;				// 中点
	Vector2 prevPlayerDirection_;	// プレイヤーとの過去の方向
	Vector3 color_;					// 色
	State state_;					// 状態
	EnemyAnimation2D animation_;	// アニメーション
};

#endif