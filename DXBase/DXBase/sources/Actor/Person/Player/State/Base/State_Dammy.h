#pragma once

#include "State.h"

class State_Dammy : public State {
public:
	// コンストラクタ
	State_Dammy(){}
	// 全てのステートに共通する初期化処理
	virtual void common_init(Actor& actor, const StateElement& element) override {}
	// 入力処理
	virtual void input(float deltaTime) override {}
	// 各状態独自の初期化
	virtual void unique_init() override{}
	// 更新処理
	virtual void update(float deltaTime) override {}
	// 更新処理
	virtual void common_update(float deltaTime) override {}
	// 衝突処理
	virtual void collide(const Actor & other) override {}
	// 描画処理
	virtual void draw()const override{}
	// 終了時の処理
	virtual void end() override {}
};
