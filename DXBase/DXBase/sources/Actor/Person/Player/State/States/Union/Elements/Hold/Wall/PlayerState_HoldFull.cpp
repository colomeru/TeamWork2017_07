#include "PlayerState_HoldFull.h"

// コンストラクタ
PlayerState_HoldFull::PlayerState_HoldFull(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) : 
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Wall(butty, retty),
	PlayerState_HoldBase_Full(butty, retty) {}

void PlayerState_HoldFull::onInit(){
	PlayerState_HoldBase_Full::onInit();
}

void PlayerState_HoldFull::onUpdate(float deltaTime){
	PlayerState_HoldBase_Wall::onUpdate(deltaTime);

}

void PlayerState_HoldFull::onEnd(){
	PlayerState_HoldBase_Full::onEnd();
}

// パッド入力処理
void PlayerState_HoldFull::onInput(float deltaTime){
	PlayerState_HoldBase_Wall::onInput(deltaTime);
	PlayerState_HoldBase_Full::onInput(deltaTime);
}

void PlayerState_HoldFull::onMove(float deltaTime){
	PlayerState_HoldBase_Wall::onMove(deltaTime);
}


