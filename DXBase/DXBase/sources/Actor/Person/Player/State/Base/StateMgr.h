#pragma once

#include "IStatePtr.h"
#include "IState.h"
#include "../../../../Base/Actor.h"
#include <unordered_map>

// ステートの処理管理
class StateMgr{
public:
	// コンストラクタ
	StateMgr();
	// コンストラクタ
	virtual ~StateMgr();
public:
	// 更新処理
	void action(Actor& actor, float deltaTime) ;
	// 衝突処理
	void collide(const Actor & other);
	// 描画処理
	void draw()const;
	// 現在のステート
	bool currentState(const int& state = 0)const;
	// 現在のステート
	bool currentActionType(const ActionType& type = ActionType::None);
	// ステートの追加
	template <class STATE>
	void add(const STATE id, const IStatePtr& state);
	// ステートの変更処理
	template <class STATE>
	void change(Actor & actor, const STATE id, const ActionType type = ActionType::None);
private:
	// コピー禁止
	StateMgr(const StateMgr& other) = delete;
	StateMgr& operator = (const StateMgr& other) = delete;
private:
	// ステート登録用map
	std::unordered_map<int, IStatePtr> states_;
	// 現在のステート
	IStatePtr currentState_;
	// 現在の要素
	IState::StateElement element_;
};

template<class STATE>
inline void StateMgr::add(const STATE id, const IStatePtr & state){
	states_[static_cast<int>(id)] = state;
}

template<class STATE>
inline void StateMgr::change(Actor & actor, const STATE id, const ActionType type){
	// 前ステートの終了処理
	currentState_->end();
	// 要素の格納
	element_.state_ = static_cast<int>(id);
	element_.type_ = type;
	// 実行ステートを変更
	currentState_ = states_[element_.state_];
	// 共通の初期化
	currentState_->common_init(actor, element_);
	// 固有の初期化
	currentState_->unique_init();
}
