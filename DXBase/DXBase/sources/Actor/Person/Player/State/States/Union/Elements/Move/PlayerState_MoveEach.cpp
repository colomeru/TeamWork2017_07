#include "PlayerState_MoveEach.h"

#include "../../../../../../../../Define.h"
#include "../../../../../../../../Input/InputMgr.h"

// コンストラクタ
PlayerState_MoveEach::PlayerState_MoveEach(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

// 状態固有の初期化
void PlayerState_MoveEach::unique_init(){

	if (element_.type_ == ActionType::Right) {
		manuBody_ = butty_;
		autoBody_ = retty_;
	}
	if (element_.type_ == ActionType::Left) {
		manuBody_ = retty_;
		autoBody_ = butty_;
	}

	// buttyのアニメーションをMOVEに変更
	butty_->animation().change(PlayerAnimID::MOVE, 2.0f);
	// rettyのアニメーションをMOVEに変更
	retty_->animation().change(PlayerAnimID::MOVE, 2.0f);
}

// 更新処理
void PlayerState_MoveEach::update(float deltaTime){
	// アニメーション反転
	flip_animation();

	// 移動処理
	move(deltaTime);

	// ボスステージ専用イベント時にEvent状態に変更
	if (cntr_->getWorld()->isEntered()) change(PlayerState_Enum_Union::EVENT);
}

// 終了時処理
void PlayerState_MoveEach::end(){}

// パッド入力処理
void PlayerState_MoveEach::input(float deltaTime){
	// buttyの入力による反転処理
	if (InputMgr::GetInstance().PadVector(element_.type_).x > 0) manuBody_->animation().change_dir(PlayerAnimID::TURN, ActionType::Right);
	if (InputMgr::GetInstance().PadVector(element_.type_).x < 0) manuBody_->animation().change_dir(PlayerAnimID::TURN, ActionType::Left);

	// 入力による移動処理
	manuBody_->position() += InputMgr::GetInstance().PadVector(element_.type_).Horizontal() * manuBody_->velocity() * PLAYER_SPEED * manuBody_->dump() * time(deltaTime);

	// ジャンプ処理
	if (jump_pad()) change(PlayerState_Enum_Union::JUMP);
	// 移動入力がない場合IDLEに変更
	else if (!move_padR() && !move_padL()) change(PlayerState_Enum_Union::IDLE);
	// 両移動キーが入力された場合状態をMOVE_BOTHに変更
	else if (InputMgr::GetInstance().AnalogPadVectorR().Length() > 0 && InputMgr::GetInstance().AnalogPadVectorL().Length() > 0)change(PlayerState_Enum_Union::MOVE_BOTH);
	else if (holdable_padR()) change(PlayerState_Enum_Union::HOLD, ActionType::Left);
	else if (holdable_padL()) change(PlayerState_Enum_Union::HOLD, ActionType::Right);
}

// 移動処理
void PlayerState_MoveEach::move(float deltaTime) {
	// buttyに重力を掛ける
	butty_->position() += gravity(deltaTime) * butty_->velocity();
	// rettyに重力を掛ける
	retty_->position() += gravity(deltaTime) * retty_->velocity();

	// buttyの追跡計算
	if (element_.type_ == ActionType::Right) chase(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);
	// rettyの追跡計算
	if (element_.type_ == ActionType::Left)  chase(butty_->position(), 0);
	
	// buttyの移動制限
	butty_->position() = clamp(butty_->position(), 0);
	// rettyの移動制限
	retty_->position() = clamp(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);
}

void PlayerState_MoveEach::flip_animation() {
	if (manuBody_->getPosition().x > autoBody_->getPosition().x) {
		autoBody_->animation().change_dir(PlayerAnimID::TURN, ActionType::Right);
	}
	else {
		autoBody_->animation().change_dir(PlayerAnimID::TURN, ActionType::Left);
	}
}

