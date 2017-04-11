#include "PlayerState_HoldAirBegin.h"

// コンストラクタ
PlayerState_HoldAirBegin::PlayerState_HoldAirBegin(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Air(butty, retty),
	PlayerState_HoldBase_Begin(butty, retty) {}

void PlayerState_HoldAirBegin::onInit(){
	PlayerState_HoldBase_Begin::onInit();
}

void PlayerState_HoldAirBegin::onUpdate(float deltaTime){
	PlayerState_HoldBase_Air::onUpdate(deltaTime);
}

void PlayerState_HoldAirBegin::onEnd(){
	PlayerState_HoldBase_Begin::onEnd();
}

void PlayerState_HoldAirBegin::onInput(float deltaTime){
	PlayerState_HoldBase_Begin::onInput(deltaTime);
}

void PlayerState_HoldAirBegin::onMove(float deltaTime){
	PlayerState_HoldBase_Air::onMove(deltaTime);
}

