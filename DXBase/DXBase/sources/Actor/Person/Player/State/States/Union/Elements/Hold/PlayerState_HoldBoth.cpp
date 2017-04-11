#include "PlayerState_HoldBoth.h"

#include "../../../../../../../../Input/InputMgr.h"

PlayerState_HoldBoth::PlayerState_HoldBoth(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

void PlayerState_HoldBoth::unique_init() {
	butty_->animation().change(PlayerAnimID::HOLD);
	retty_->animation().change(PlayerAnimID::HOLD);

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_SYOUTOTU), DX_PLAYTYPE_BACK);
}

void PlayerState_HoldBoth::update(float deltaTime) {}

void PlayerState_HoldBoth::end() {}

void PlayerState_HoldBoth::input(float deltaTime){
	if (!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1)) {
		change(PlayerState_Enum_Union::HOLD_BEGIN, ActionType::Right);
	}
	if (!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1)) {
		change(PlayerState_Enum_Union::HOLD_BEGIN, ActionType::Left);
	}
}

