#include "PlayerState_HoldItem.h"

#include "../../../../../../../../Input/InputMgr.h"
#include "../../../../../../../../Define.h"
#include "../../../../../../../Base/ActorGroup.h"

#include "../../../../../PlayerBodyCollider.h"

PlayerState_HoldItem::PlayerState_HoldItem(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Wall(butty, retty) {}

void PlayerState_HoldItem::onInit(){
	cntr_->getWorld()->addActor(ActorGroup::Player_Collider, std::make_shared<PlayerBodyCollider>(cntr_->getWorld(), std::string("PlayerHold")));
}

void PlayerState_HoldItem::onUpdate(float deltaTime){
	auto holdColl = cntr_->world_->findActor("PlayerHoldCollider");
	if (holdColl != nullptr) holdColl->position() = moveBody_->getPosition();
}

void PlayerState_HoldItem::onEnd(){
	auto holdColl = cntr_->world_->findActor("PlayerHoldCollider");
	if (holdColl != nullptr)holdColl->dead();
}

void PlayerState_HoldItem::onInput(float deltaTime){
	// ˆÚ“®‚Ì“ü—Í‚ª‚È‚­‚È‚Á‚½ê‡IDLEó‘Ô‚É‘JˆÚ
	if ((!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1) && element_.type_ == ActionType::Right) ||
		(!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1) && element_.type_ == ActionType::Left)) {
		change(PlayerState_Enum_Union::HOLD_BEGIN, element_.type_);
	}

	if ((!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1) && element_.type_ == ActionType::Left) ||
		(!InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1) && element_.type_ == ActionType::Right)) {
		change(PlayerState_Enum_Union::IDLE);
	}
}

