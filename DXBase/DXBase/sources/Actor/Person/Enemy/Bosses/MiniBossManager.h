#ifndef MINI_BOSS_MANAGER_H_
#define MINI_BOSS_MANAGER_H_

#include <list>

class MiniBoss;

class MiniBossManager {
public:
	MiniBossManager();
	void update(float deltaTime);
	// ミニボスの追加を行います
	void addMiniBoss(MiniBoss* miniBoss);
	// ミニボス全体の混乱状態を解除します
	void backToSanity();
	// ミニボス全体がボスの位置にいるかを返します
	bool isMiniBossAllBP();
	// ミニボスをボス持ち上げ状態にします
	void bossLift();
	// 持ち上げ終わったか
	bool isLiftEnd();
	// 持ち上げ移動速度の設定
	void LiftMove(const float speed);

private:
	typedef std::list<MiniBoss*> MiniBossContainer;
	MiniBossContainer miniBosses_;
};

#endif