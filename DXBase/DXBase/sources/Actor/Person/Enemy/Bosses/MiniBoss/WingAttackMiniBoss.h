#ifndef WING_ATTACK_MINI_BOSS_H_
#define WING_ATTACK_MINI_BOSS_H_

#include "FighterMiniBossh.h"
#include <map>
#include <random>

class WingAttackMiniBoss : public FighterMiniBoss {
public:
	WingAttackMiniBoss(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale = 64.0f);
	/*void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;*/

private:
	// ‘Ò‹@ó‘Ô
	void idel(float deltaTime) override;
	// ˆÚ“®ó‘Ô
	void move(float deltaTime) override;
	//  
	void attack(float deltaTime) override;
	// €–Só‘Ô
	void deadMove(float deltaTime) override;
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì’Ç‰Á
	void addAnimation() override;
	// ‰HUŒ‚
	void wingAttack();

private:
	int attackCount_;			// UŒ‚‰ñ”
	bool isAttack_;				// UŒ‚‚µ‚½‚©
	Vector2 direction_;			// •ûŒü
	std::mt19937 mt_;			// —”‚Ì‰Šúseed
	// ‰H‚Ì‰ñ“]ƒRƒ“ƒeƒi
	typedef std::map<int, float> WingDegreeMap;
	WingDegreeMap wingDegrees;
};

#endif