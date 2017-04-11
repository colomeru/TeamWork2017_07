#include "PlayerState_Single.h"

#include "../../../../../../Input/InputMgr.h"
#include "../../../../../../Define.h"
#include "../../../Effect/PlayerEffectObj.h"

// コンストラクタ
PlayerState_Single::PlayerState_Single(const Keys& keys) :
	keys_(keys) {
}

// 全てのステートに共通する初期化処理
void PlayerState_Single::common_init(Actor & actor, const StateElement & element){
	element_ = element;
	timer_ = 0;
	isEnd_ = false;

	body_ = dynamic_cast<PlayerBody*>(&actor);
}

void PlayerState_Single::input(float deltaTime){
	//InputMgr::GetInstance().isConnectGamePad() ? pad_input(deltaTime) : key_input(deltaTime);
}

void PlayerState_Single::common_update(float deltaTime){

	timer_ += deltaTime;


	body_->collider();
	body_->velocity() = Vector2::One;
}

// 衝突処理

void PlayerState_Single::collide(const Actor & other) {}

// 描画処理
void PlayerState_Single::draw() const {}

bool PlayerState_Single::is_butty() const{
	return keys_.right == KeyCode::RIGHT;
}

bool PlayerState_Single::is_retty()const {
	return keys_.right == KeyCode::D;
}
