#include "PlayerState_HoldBase_Swim.h"

#include "../../../../../../../../../Input/InputMgr.h"

// コンストラクタ
PlayerState_HoldBase_Swim::PlayerState_HoldBase_Swim(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : PlayerState_HoldBase(butty, retty) {}

// 状態固有の初期化
void PlayerState_HoldBase_Swim::onInit(){
	// くっつき移動中効果音再生
	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_NOBI), DX_PLAYTYPE_LOOP);
}

// 終了時処理
void PlayerState_HoldBase_Swim::onEnd(){
	// くっつき移動中効果音停止
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_NOBI));
}

// パッド入力処理
void PlayerState_HoldBase_Swim::onInput(float deltaTime){
	// くっつきボタンが離れた場合IDLE状態に遷移
	if (hold_released()) change(PlayerState_Enum_Union::IDLE);
}
