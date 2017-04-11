#include "PlayerState_MoveBoth.h"

#include "../../../../../../../../Define.h"
#include "../../../../../../../../Input/InputMgr.h"

// コンストラクタ
PlayerState_MoveBoth::PlayerState_MoveBoth(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

// 状態固有の初期化
void PlayerState_MoveBoth::unique_init(){}

// 更新処理
void PlayerState_MoveBoth::update(float deltaTime){
	// 移動処理
	move(deltaTime);

	// ボスステージ専用イベント時にEvent状態に変更
	if (cntr_->getWorld()->isEntered()) change(PlayerState_Enum_Union::EVENT);
}

// 終了時処理
void PlayerState_MoveBoth::end(){}

// 入力処理
void PlayerState_MoveBoth::input(float deltaTime) {
	// buttyの入力による反転処理
	if (InputMgr::GetInstance().AnalogPadVectorR().x > 0) butty_->animation().change_dir(PlayerAnimID::TURN, ActionType::Right);
	if (InputMgr::GetInstance().AnalogPadVectorR().x < 0) butty_->animation().change_dir(PlayerAnimID::TURN, ActionType::Left);

	// rettyの入力による反転処理
	if (InputMgr::GetInstance().AnalogPadVectorL().x > 0) retty_->animation().change_dir(PlayerAnimID::TURN, ActionType::Right);
	if (InputMgr::GetInstance().AnalogPadVectorL().x < 0) retty_->animation().change_dir(PlayerAnimID::TURN, ActionType::Left);

	// 入力による移動処理
	butty_->position() += InputMgr::GetInstance().AnalogPadVectorR().Horizontal() * speed_b(deltaTime);
	retty_->position() += InputMgr::GetInstance().AnalogPadVectorL().Horizontal() * speed_r(deltaTime);

	// ジャンプ処理
	if (jump_pad()) change(PlayerState_Enum_Union::JUMP);
	// 片方の入力がない場合MOVEに変更
	else if (!move_padR()) change(PlayerState_Enum_Union::MOVE, ActionType::Left);
	else if (!move_padL()) change(PlayerState_Enum_Union::MOVE, ActionType::Right);
	else if (holdable_padR()) change(PlayerState_Enum_Union::HOLD, ActionType::Left);
	else if (holdable_padL()) change(PlayerState_Enum_Union::HOLD, ActionType::Right);

}

// 移動処理
void PlayerState_MoveBoth::move(float deltaTime){
	// buttyに重力を掛ける
	butty_->position() += gravity(deltaTime) * butty_->velocity();
	// rettyに重力を掛ける
	retty_->position() += gravity(deltaTime) * retty_->velocity();
	// buttyの移動制限
	butty_->position() = clamp(butty_->position(), 0);
	// rettyの移動制限
	retty_->position() = clamp(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);
}
