#include "PlayerState_HoldAirSwim.h"

// コンストラクタ
PlayerState_HoldAirSwim::PlayerState_HoldAirSwim(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Air(butty, retty),
	PlayerState_HoldBase_Swim(butty, retty){}

void PlayerState_HoldAirSwim::onInit(){
	PlayerState_HoldBase_Swim::onInit();
}

void PlayerState_HoldAirSwim::onUpdate(float deltaTime){
	PlayerState_HoldBase_Air::onUpdate(deltaTime);
}

void PlayerState_HoldAirSwim::onEnd(){
	PlayerState_HoldBase_Swim::onEnd();
}

void PlayerState_HoldAirSwim::onInput(float deltaTime){
	PlayerState_HoldBase_Swim::onInput(deltaTime);
}

void PlayerState_HoldAirSwim::onMove(float deltaTime){
	PlayerState_HoldBase_Air::onMove(deltaTime);
}
