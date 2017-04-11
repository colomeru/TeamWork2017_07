#ifndef WALL_ATTACK_H_
#define WALL_ATTACK_H_

#include "BossAttack.h"
#include <vector>

// 壁移動攻撃
class WallAttack : public BossAttack {
private:
	enum class State {
		FloorSearch,
		FloorGetOff,
		WallMove,
		WallAttackMove
	};

public:
	WallAttack();
	WallAttack(IWorld* world, const Vector2& position);
	// 攻撃
	virtual void attack(float deltaTime) override;
	// 攻撃行動のリフレッシュを行います
	virtual void Refresh() override;

private:
	// 床捜索状態です
	void floorSearch(float deltaTime);
	// 床から降りる状態です
	void floorGetOff(float deltaTime);
	// 壁移動状態です
	void wallMove(float deltaTime);
	// 壁攻撃状態です
	void wallAttackMove(float deltaTime);
	// 壁移動する時間を決定します
	void setAttackSecond();
	// 状態の変更を行います
	void changeState(State state, int number);

protected:
	int count_;						// 方向転換カウント
	int aSecond_;					// 攻撃状態に遷移する秒数です
	int createCount_;				// エフェクト生成回数
	float speed_;					// 移動速度
	bool isWallAttackEnd_;			// 壁攻撃が終わったか
	State state_;					// 状態
	Vector2 prevPlayerDistance_;	// 過去のプレイヤーの方向
	// 壁移動時間コンテナ
	typedef std::vector<int> MoveTimeContainer;
	MoveTimeContainer moveTimes_;
	// 方向コンテナ
	typedef std::vector<float> MoveDirectionContainer;
	MoveDirectionContainer moveDirections_;
};

#endif
