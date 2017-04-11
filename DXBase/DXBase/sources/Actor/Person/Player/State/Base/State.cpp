#include "State.h"

#include "../../../../../Math/Math.h"
#include "../../../../../Define.h"

// コンストラクタ
State::State() :
	element_(-1),
	next_element_(-1),
	timer_(0),
	isEnd_(false){}

// ステートの変更処理
void State::request(const StateElement & element){
	if (element_.state_ == element.state_)return;
	next_element_ = element;
	isEnd_ = true;
}

// ステートが終了したか否か
bool State::isEnd(){
	return isEnd_;
}

// 次のステートの要素
IState::StateElement State::next() const{
	return next_element_;
}

// 重力
Vector2 State::gravity(float deltaTime) const{
	return 	Vector2::Up * GRAVITY * time(deltaTime);
}

// 時間
float State::time(float deltaTime) const{
	return deltaTime * static_cast<float>(GetRefreshRate());
}
