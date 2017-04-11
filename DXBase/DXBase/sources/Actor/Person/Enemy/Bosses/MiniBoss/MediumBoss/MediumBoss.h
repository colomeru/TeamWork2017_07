#ifndef MEDIUM_BOSS_H_
#define MEDIUM_BOSS_H_

#include "../FighterMiniBossh.h"
#include <random>

class BossGaugeUI;

class MediumBoss : public FighterMiniBoss {
public:
	MediumBoss(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale = 64.0f);
	virtual void onUpdate(float deltaTime) override;
	void onDraw()const override;

protected:
	// 勝負待機状態
	virtual void battleIdel(float deltaTime) override;
	// 待機状態
	virtual void idel(float deltaTime) override;
	// 移動状態
	virtual void move(float deltaTime) override;
	// 攻撃状態
	virtual void attack(float deltaTime) override;
	// 死亡状態
	virtual void deadMove(float deltaTime) override;
	// 床に当たった時の処理
	virtual void floorHit() override;
	// プレイヤーの攻撃に当たった時の処理
	virtual void playerAttackHit(Actor & actor) override;
	// ランダムの値を取得します
	int getRandomInt(const int min, const int max);

protected:
	int hp_;					// 体力
	int damage_;				// ダメージ量
	Vector2 direction_;			// 方向
	BossGaugeUI* bossGaugeUI_;	// ゲージUI

private:
	std::mt19937 mt_;			// 乱数の初期seed
};

#endif