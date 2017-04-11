#include "PlayerState_HoldAirFull.h"

// コンストラクタ
PlayerState_HoldAirFull::PlayerState_HoldAirFull(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : 
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Air(butty, retty),
	PlayerState_HoldBase_Full(butty, retty) {}

void PlayerState_HoldAirFull::onInit(){
	PlayerState_HoldBase_Full::onInit();
}

void PlayerState_HoldAirFull::onUpdate(float deltaTime){
	PlayerState_HoldBase_Air::onUpdate(deltaTime);
}

void PlayerState_HoldAirFull::onEnd(){
	PlayerState_HoldBase_Full::onEnd();
}

void PlayerState_HoldAirFull::onInput(float deltaTime){
	PlayerState_HoldBase_Full::onInput(deltaTime);
}

void PlayerState_HoldAirFull::onMove(float deltaTime){
	PlayerState_HoldBase_Air::onMove(deltaTime);
}

