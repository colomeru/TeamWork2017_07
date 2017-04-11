#ifndef STAGE2_MINI_BOSS_H_
#define STAGE2_MINI_BOSS_H_

#include "MediumBoss.h"
#include <vector>

class Stage2MiniBoss : public MediumBoss{
public:
	Stage2MiniBoss(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale = 96.0f);

private:
	// 待機状態
	void idel(float deltaTime) override;
	// 移動状態
	void move(float deltaTime) override;
	// 攻撃状態
	void attack(float deltaTime) override;
	// 死亡状態
	void deadMove(float deltaTime) override;
	// 床に当たった時の処理
	void floorHit() override;
	// ぴより行動
	void piyoriMove(float deltaTime);
	// 戻り行動
	void returnMove(float deltaTime);

private:
	float movePositionCount_;		// 移動した合計数
	float effectCreateTimer_;		// スターエフェクト生成時間
	bool isEffectCreate_;			// スターエフェクトを生成したか
	bool isReturn_;					// 指定の位置に戻るか
	Vector2 prevAttackPosition_;	// 攻撃する前の位置
	// スターエフェクト用コンテナ
	typedef std::list<ActorPtr> StarContainer;
	StarContainer stars_;
		//stars_
	//Vector2 prevPlayerDirection_;	// 前回のプレイヤーとの方向
};

#endif
