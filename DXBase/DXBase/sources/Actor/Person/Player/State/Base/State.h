#pragma once

#include "IState.h"

struct Vector2;

// 各プレイヤーステートの基底クラス
class State : public IState {
public:
	// コンストラクタ
	State();
protected:
	// ステートの変更処理
	virtual void request(const StateElement& element) override;
	// ステートが終了したか否か
	virtual bool isEnd() override;
	// 次のステートの要素
	virtual StateElement next() const override;
protected:
	// 重力
	Vector2 gravity(float deltaTime) const;
	// 時間
	float time(float deltaTime) const;
protected:
	// ステートの要素
	StateElement element_;
	// ステートの要素
	StateElement next_element_;
	// ステートが終了したか否か
	bool isEnd_;
	// タイマー
	float timer_;
};