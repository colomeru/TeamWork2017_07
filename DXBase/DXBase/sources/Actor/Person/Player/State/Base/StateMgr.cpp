#include "StateMgr.h"

#include "State_Dammy.h"

#include "../../../../../World/IWorld.h"

// コンストラクタ
StateMgr::StateMgr() : currentState_(std::make_shared<State_Dammy>()), element_(-1){}

StateMgr::~StateMgr(){}

// 更新処理
void StateMgr::action(Actor& actor, float deltaTime) {
	// 入力処理
	if(!actor.getWorld()->GetPlayerNotMove()) currentState_->input(deltaTime);
	// 更新処理
	currentState_->update(deltaTime);
	// 更新処理
	currentState_->common_update(deltaTime);
	// 終了判定がtrueになった場合ステートを変更
	if (currentState_->isEnd()) change(actor, currentState_->next().state_, currentState_->next().type_);
}

// 衝突処理
void StateMgr::collide(const Actor & other){
	// 衝突処理
	currentState_->collide(other);
}

// 描画処理
void StateMgr::draw() const{
	// 描画処理
	currentState_->draw();
}

// 現在のステート
bool StateMgr::currentState(const int& state) const {
	return element_.state_ == state;
}

bool StateMgr::currentActionType(const ActionType& type){
	return element_.type_ == type;
}


