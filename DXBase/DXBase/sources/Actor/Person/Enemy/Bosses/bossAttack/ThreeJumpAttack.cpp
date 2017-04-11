#include "ThreeJumpAttack.h"

ThreeJumpAttack::ThreeJumpAttack() :
	JumpAttack(nullptr, Vector2::Zero),
	jumpCount_(0),
	initJumpCount_(jumpCount_)
{}

ThreeJumpAttack::ThreeJumpAttack(IWorld* world, const Vector2 & position) :
	JumpAttack(world, position),
	jumpCount_(3),
	initJumpCount_(jumpCount_)
{}

void ThreeJumpAttack::attack(float deltaTime)
{
	JumpAttack::attack(deltaTime);
	// ジャンプ後の待機状態が終わったら、再度ジャンプ攻撃
	if (!isJumpEnd_) return;
	// カウントを減らす
	jumpCount_--;
	JumpAttack::Refresh();
	// ジャンプ回数が0なら攻撃終了
	if (jumpCount_ == 0) {
		isAttackEnd_ = true;
		return;
	}
	// まだジャンプ回数があるなら、再度ジャンプ
	isJump_ = false;
	isJumpEnd_ = false;
	recastTimer_ = initRecastTimer_;
}

void ThreeJumpAttack::Refresh()
{
	JumpAttack::Refresh();
	isAttackHit_ = true;
	jumpCount_ = initJumpCount_;
}
