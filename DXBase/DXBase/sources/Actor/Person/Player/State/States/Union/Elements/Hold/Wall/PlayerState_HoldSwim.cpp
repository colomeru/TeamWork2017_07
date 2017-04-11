#include "PlayerState_HoldSwim.h"

// コンストラクタ
PlayerState_HoldSwim::PlayerState_HoldSwim(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Wall(butty, retty),
	PlayerState_HoldBase_Swim(butty, retty) {}

void PlayerState_HoldSwim::onInit(){
	PlayerState_HoldBase_Swim::onInit();

}

void PlayerState_HoldSwim::onUpdate(float deltaTime){
	PlayerState_HoldBase_Wall::onUpdate(deltaTime);
}

void PlayerState_HoldSwim::onEnd(){
	PlayerState_HoldBase_Swim::onEnd();
}

// パッド入力処理
void PlayerState_HoldSwim::onInput(float deltaTime){
	PlayerState_HoldBase_Swim::onInput(deltaTime);
	PlayerState_HoldBase_Wall::onInput(deltaTime);
}

void PlayerState_HoldSwim::onMove(float deltaTime){
	PlayerState_HoldBase_Wall::onMove(deltaTime);
}


