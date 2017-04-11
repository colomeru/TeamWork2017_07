#ifndef STAGE1_MINI_BOSS_H_
#define STAGE1_MINI_BOSS_H_

#include "MediumBoss.h"
#include <map>

class Stage1MiniBoss : public MediumBoss {
public:
	Stage1MiniBoss(
		IWorld* world,
		const Vector2& position,
		const float bodyScale = 96.0f);

private:
	// ‘Ò‹@ó‘Ô
	void idel(float deltaTime) override;
	// ˆÚ“®ó‘Ô
	void move(float deltaTime) override;
	// UŒ‚ó‘Ô
	void attack(float deltaTime) override;
	// ‰HUŒ‚
	void wingAttack();

private:
	int attackCount_;			// UŒ‚‰ñ”
	bool isAttack_;				// UŒ‚‚µ‚½‚©
	Vector2 direction_;			// •ûŒü
	// ‰H‚Ì‰ñ“]ƒRƒ“ƒeƒi
	typedef std::map<int, float> WingDegreeMap;
	WingDegreeMap wingDegrees;
};

#endif
