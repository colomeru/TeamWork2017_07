#ifndef BOSS_EFFECT_H_
#define BOSS_EFFECT_H_

#include "../../../../Base/Actor.h"
#include "../../../../../ResourceLoader/ResourceLoader.h"
#include "../../../../../Animation/Enemy/EnemyAnimation2D.h"

class BossEffect : public Actor {
protected:
	enum {
		EFFECT_ATTACK = 0,
		EFFECT_BOKO = 1,
		EFFECT_DASH = 2,
		EFFECT_PIYO = 3,
		EFFECT_ENTRY_SIGN = 4,
		EFFECT_MINI_BOSS = 5,
		EFFECT_ENEMY_DEAD = 6,
		EFFECT_ENEMY_COLLIDE = 7,
	};

public:
	BossEffect(
		IWorld* world,
		std::string name,
		const Vector2& position,
		const Vector2& texSize,
		const int animeNum,
		const AnimationID id,
		const bool isLoop = false);
	~BossEffect();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;

//protected:
//	// アニメーションの追加を行います
//	virtual void addAnimation();

protected:
	EnemyAnimation2D animation_;

	//int alpha_;			// 透明度
	float scale_;		// 大きさ
	bool isLoop_;		// ループするか
	Vector2 texSize_;	// テクスチャの大きさ
};

#endif
