#pragma once

#include "ActionType.h"

class Actor;

class IState {
public:
	// ステートの要素
	struct StateElement {
		// ステート
		int state_;
		// タイプ
		ActionType type_;
		// コンストラクタ
		StateElement(const int& state, const ActionType& type = ActionType::None) : state_(state), type_(type){}
	};
public:
	// 仮想デストラクタ
	virtual ~IState() {}
	// 全状態共通の初期化
	virtual void common_init(Actor& actor, const StateElement& element = StateElement({ -1, ActionType::None })) = 0;
	// 各状態独自の初期化
	virtual void unique_init() = 0;
	// 入力処理
	virtual void input(float deltaTime) = 0;
	// 更新処理
	virtual void update(float deltaTime) = 0;
	// 更新処理
	virtual void common_update(float deltaTime) = 0;
	// 衝突処理
	virtual void collide(const Actor& other) = 0;
	// 描画処理
	virtual void draw()const = 0;
	// 終了時の処理
	virtual void end() = 0;
	// 終了を返す
	virtual bool isEnd() = 0;
	// 状態の変更
	virtual void request(const StateElement& element) = 0;
	// 次のステートの要素
	virtual StateElement next() const = 0;
};
