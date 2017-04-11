#ifndef STAGE3_MINI_BOSS_H_
#define STAGE3_MINI_BOSS_H_

#include "MediumBoss.h"
#include <vector>

class FloorSearchPoint;

class Stage3MiniBoss : public MediumBoss {
public:
	Stage3MiniBoss(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale = 96.0f);
	void onUpdate(float deltaTime) override;

private:
	// 勝負待機状態
	void battleIdel(float deltaTime) override;
	// 待機状態
	void idel(float deltaTime) override;
	// 移動状態
	void move(float deltaTime) override;
	// 攻撃状態
	void attack(float deltaTime) override;
	// 死亡状態
	void deadMove(float deltaTime) override;
	// ぴより行動
	void piyoriMove(float deltaTime);
	// 休み行動
	void restMove(float deltaTime);
	// プレイヤーの攻撃に当たった時の処理
	void playerAttackHit(Actor& actor) override;
	// 壁移動攻撃処理
	void wallAttack(float deltaTime);
	// 浮遊ミニボスの生成を行います
	void createMiniBoss(float deltaTime);

private:
	int wallCount_;					// 壁移動カウント
	float effectCreateTimer_;		// スターエフェクト生成時間
	float groundTimer_;				// 接地時間
	float mbTimer_;					// ミニボス生成時間
	bool isEffectCreate_;			// スターエフェクトを生成したか
	bool isPiyori_;					// ぴよるか
	bool isPrevWspHit_;
	Vector2 prevAttackPosition_;	// 攻撃する前の位置
	FloorSearchPoint* wspObj_;		// 壁捜索オブジェクト

	// 方向コンテナ
	typedef std::vector<float> MoveDirectionContainer;
	MoveDirectionContainer moveDirections_;
	// スターエフェクト用コンテナ
	typedef std::list<ActorPtr> StarContainer;
	StarContainer stars_;
};

#endif
