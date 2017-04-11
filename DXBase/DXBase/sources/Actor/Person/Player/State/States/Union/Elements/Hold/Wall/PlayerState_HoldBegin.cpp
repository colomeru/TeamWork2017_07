#include "PlayerState_HoldBegin.h"

// コンストラクタ
PlayerState_HoldBegin::PlayerState_HoldBegin(const PlayerBodyPtr & butty, const PlayerBodyPtr & retty) :
	PlayerState_HoldBase(butty, retty),
	PlayerState_HoldBase_Wall(butty, retty),
	PlayerState_HoldBase_Begin(butty, retty) {}

void PlayerState_HoldBegin::onInit(){
	PlayerState_HoldBase_Begin::onInit();
}

void PlayerState_HoldBegin::onUpdate(float deltaTime){
	PlayerState_HoldBase_Wall::onUpdate(deltaTime);
}

void PlayerState_HoldBegin::onEnd(){
	PlayerState_HoldBase_Begin::onEnd();
}

// パッド入力処理
void PlayerState_HoldBegin::onInput(float deltaTime){
	PlayerState_HoldBase_Wall::onInput(deltaTime);
	PlayerState_HoldBase_Begin::onInput(deltaTime);
}

void PlayerState_HoldBegin::onMove(float deltaTime){
	PlayerState_HoldBase_Wall::onMove(deltaTime);
}

