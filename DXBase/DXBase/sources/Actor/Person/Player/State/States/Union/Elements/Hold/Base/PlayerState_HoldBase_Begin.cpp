#include "PlayerState_HoldBase_Begin.h"

#include "../../../../../../../../../Input/InputMgr.h"

// コンストラクタ
PlayerState_HoldBase_Begin::PlayerState_HoldBase_Begin(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : PlayerState_HoldBase(butty, retty) {}

// 状態固有の初期化
void PlayerState_HoldBase_Begin::onInit(){
	// アニメーションの遷移
	moveBody_->animation().change(PlayerAnimID::SWIM);
	holdBody_->animation().change(PlayerAnimID::HOLD);

	//// くっつき時効果音再生
	//PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_SYOUTOTU), DX_PLAYTYPE_BACK);
}

// 終了時処理
void PlayerState_HoldBase_Begin::onEnd(){}

// パッド入力処理
void PlayerState_HoldBase_Begin::onInput(float deltaTime){
	// くっつきボタンが離れた場合IDLE状態に遷移
	if (hold_released()) change(PlayerState_Enum_Union::IDLE);
}
