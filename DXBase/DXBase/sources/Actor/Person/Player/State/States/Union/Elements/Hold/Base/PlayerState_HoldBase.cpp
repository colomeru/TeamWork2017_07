#include "PlayerState_HoldBase.h"

#include "../../../../../../../../../Define.h"
#include "../../../../../../../../../Input/InputMgr.h"
#include "../../../../../../../../../Scene/Base/SceneDataKeeper.h"

// コンストラクタ
PlayerState_HoldBase::PlayerState_HoldBase(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : PlayerState_Union(butty, retty) {}

// 状態固有の初期化
void PlayerState_HoldBase::unique_init(){
	// くっついているプレイヤーと移動しているプレイヤーの設定
	//rettyがくっついている場合
	if (element_.type_ == ActionType::Right) {
		holdBody_ = retty_;
		moveBody_ = butty_;
	}
	//buttyがくっついている場合
	if (element_.type_ == ActionType::Left) {
		holdBody_ = butty_;
		moveBody_ = retty_;
	}

	// 継承先の初期化処理	
	onInit();
}

// 更新処理	
void PlayerState_HoldBase::update(float deltaTime){
	// チャージ時間カウント更新
	cntr_->getWorld()->GetKeeper()->addChargeTime(deltaTime);

	// 継承先の更新処理	
	onUpdate(deltaTime);

	// 移動処理
	move(deltaTime);
}

// 終了時処理
void PlayerState_HoldBase::end(){
	// 継承先の終了時処理
	onEnd();
}

// 入力処理
void PlayerState_HoldBase::input(float deltaTime){
	// 入力による座標移動
	moveBody_->position() += input_vector(InputMgr::GetInstance().PadVector(element_.type_), deltaTime);

	// 入力によるアニメーション切り替え
	if (InputMgr::GetInstance().PadVector(element_.type_).x > 0) moveBody_->animation().change_dir(PlayerAnimID::SWIM_TURN, ActionType::Right);
	if (InputMgr::GetInstance().PadVector(element_.type_).x < 0) moveBody_->animation().change_dir(PlayerAnimID::SWIM_TURN, ActionType::Left);

	// 継承先の入力処理
	onInput(deltaTime);
}

// 移動処理
void PlayerState_HoldBase::move(float deltaTime) {
	// 継承先の移動処理
	onMove(deltaTime);


	// プレイヤーの移動制限
	if (element_.type_ == ActionType::Right) {
		butty_->position() = clamp(butty_->position(), 0);
	}
	if (element_.type_ == ActionType::Left) {
		retty_->position() = clamp(retty_->position(), PLAYER_CNTR_DIV_NUM - 1);
	}

}

// 移動速度計算
Vector2 PlayerState_HoldBase::input_vector(const Vector2 & input, float deltaTime){
	return input * 7.5f * moveBody_->velocity() * PLAYER_SPEED * moveBody_->dump() * time(deltaTime);
}

// くっつきボタンが離れた場合
bool PlayerState_HoldBase::hold_released() {
	// くっつきボタンが離れたかどうか
	return	(!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1) && element_.type_ == ActionType::Left) || 
			(!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1) && element_.type_ == ActionType::Right);
}




