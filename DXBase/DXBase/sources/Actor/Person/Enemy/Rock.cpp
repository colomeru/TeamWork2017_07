#include "Rock.h"
#include "Bosses/MiniBoss/ImportAnimationNumber.h"
#include "../../../ResourceLoader/ResourceLoader.h"
#include "../../../Define.h"
#include "../../../World/IWorld.h"
#include <random>

Rock::Rock(IWorld * world, const Vector2 & position, const float bodyScale) : 
	AdhereMiniBoss(world, position, bodyScale)
{
}

// 落下状態
void Rock::move(float deltaTime)
{
	// 落下
	position_.y += speed_ * stateTimer_  * 9.8f * (deltaTime * 60.0f);
	addTexPos_ = Vector2::Up * 10.0f;
	// プレイヤーの方向を向く
	animation_.changeDirType(-getPlayerDirection().x);
}

// 死亡状態
void Rock::deadMove(float deltaTime)
{
	AdhereMiniBoss::adhereDead(deltaTime);
}

// 床に当たった時の処理
void Rock::floorHit()
{
	if (state_ == State::Attack || state_ == State::Dead) return;
	changeState(State::Dead, RUN_NUMBER);
	// 位置の更新
	addTexPos_ = Vector2::Up * -50.0f;
	// 中点を下にする
	orizin_ = Vector2::One * (256.0f / 2.0f) + Vector2::Up * 100.0f;
	// プレイヤーと逆の方向を向く
	animation_.changeDirType(getPlayerDirection().x);
}
