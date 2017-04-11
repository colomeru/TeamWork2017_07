#include "PlayerState_Idle.h"

#include "../../../../../../../Define.h"
#include "../../../../../../../Scene/Base/SceneDataKeeper.h"

// コンストラクタ
PlayerState_Idle::PlayerState_Idle(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

// 状態固有の初期化
void PlayerState_Idle::unique_init(){
	// buttyのアニメーションをIDLEに変更
	butty_->animation().change(PlayerAnimID::IDLE);
	// rettyのアニメーションをIDLEに変更
	retty_->animation().change(PlayerAnimID::IDLE);

	//cntr_->getWorld()->GetKeeper()->setJumpCount(0);
	cntr_->getWorld()->GetKeeper()->comboReset(0);
}

// 更新処理
void PlayerState_Idle::update(float deltaTime) {
	// 移動処理
	move(deltaTime);

	// ボスステージ専用イベント時にEvent状態に変更
	if (cntr_->getWorld()->isEntered()) change(PlayerState_Enum_Union::EVENT);
}

// 終了時処理
void PlayerState_Idle::end(){}

// 入力処理
void PlayerState_Idle::input(float deltaTime){
	// ジャンプボタン入力時にジャンプ状態に変更
	if (jump_pad()) change(PlayerState_Enum_Union::JUMP);

	// 右スティック入力時にbutty移動状態に変更
	else if (move_padR()) change(PlayerState_Enum_Union::MOVE, ActionType::Right);
	// 左スティック入力時にbutty移動状態に変更
	else if (move_padL()) change(PlayerState_Enum_Union::MOVE, ActionType::Left);

	// buttyくっつき可能時にR1ボタンでくっつき状態に変更
	else if (holdable_padR()) change(PlayerState_Enum_Union::HOLD, ActionType::Left);
	// rettyくっつき可能時にR1ボタンでくっつき状態に変更
	else if (holdable_padL()) change(PlayerState_Enum_Union::HOLD, ActionType::Right);
}

// 移動処理
void PlayerState_Idle::move(float deltaTime) {
	// buttyに重力を掛ける
	butty_->position() += gravity(deltaTime) * butty_->velocity();
	// rettyに重力を掛ける
	retty_->position() += gravity(deltaTime) * retty_->velocity();

	// buttyの追跡計算
	chase(butty_->position(), 0);
	// rettyの追跡計算
	chase(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);

	//if (!cntr_->getWorld()->GetPlayerNotMove()) {
		// buttyの移動制限
		butty_->position() = clamp(butty_->position(), 0);
		// rettyの移動制限
		retty_->position() = clamp(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);
	//}
}



