#include "PlayerState_HoldBase_Air.h"

#include "../../../../../../../../../Define.h"

// コンストラクタ
PlayerState_HoldBase_Air::PlayerState_HoldBase_Air(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : PlayerState_HoldBase(butty, retty) {}

// 更新処理	
void PlayerState_HoldBase_Air::onUpdate(float deltaTime) {
	// 伸び具合によって状態を遷移
	if (moveBody_->distance() < PLAYER_SWIM_LENGTH) change(PlayerState_Enum_Union::HOLD_AIR, element_.type_);
	else if (moveBody_->distance() < PLAYER_MAX_STRETCH_LENGTH * 0.7f) change(PlayerState_Enum_Union::HOLD_AIR_SWIM, element_.type_);
	else change(PlayerState_Enum_Union::HOLD_AIR_FULL, element_.type_);
}

// 移動処理
void PlayerState_HoldBase_Air::onMove(float deltaTime){
	// それぞれのBodyに重力をかける
	moveBody_->position() += gravity(deltaTime) / 2 * moveBody_->velocity();
	holdBody_->position() += gravity(deltaTime) / 2 * holdBody_->velocity();
}
