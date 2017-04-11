#include "PlayerState_HoldBase_Full.h"

#include "../../../../../../../../../Input/InputMgr.h"

#include "../../../../../../Effect/PlayerEffectObj.h"
#include "../../../../../../../../Base/ActorGroup.h"

// コンストラクタ
PlayerState_HoldBase_Full::PlayerState_HoldBase_Full(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : PlayerState_HoldBase(butty, retty) {}

// 状態固有の初期化
void PlayerState_HoldBase_Full::onInit(){
	// チャージエフェクトの生成
	cntr_->world_->addActor(ActorGroup::Effect, std::make_shared<PlayerEffectObj>(cntr_->world_, cntr_->getPosition(), PlayerEffectID::SHOUGEKI, 3.0f, 5.0f));
	
	// チャージ効果音の再生
	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_NOBI_FULL), DX_PLAYTYPE_BACK);
}

// 終了時処理
void PlayerState_HoldBase_Full::onEnd(){}

// パッド入力処理
void PlayerState_HoldBase_Full::onInput(float deltaTime){
	// くっつきボタンが離れた場合Attack状態に遷移
	if (hold_released()) change(PlayerState_Enum_Union::ATTACK, element_.type_);
}

