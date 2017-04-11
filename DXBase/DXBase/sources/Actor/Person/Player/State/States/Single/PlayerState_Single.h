#pragma once

#include "../../Base/State.h"

#include "../../../PlayerBody.h"

#include "../../../../../../Input/KeyCode.h"

// 各プレイヤーステートの基底クラス
class PlayerState_Single : public State {
public:
	struct Keys {
	public:
		KeyCode up;
		KeyCode down;
		KeyCode right;
		KeyCode left;
	public:
		Keys(const KeyCode& up = KeyCode::UP, const KeyCode& down = KeyCode::DOWN, const KeyCode& right = KeyCode::RIGHT, const KeyCode& left = KeyCode::LEFT) :up(up), down(down), right(right), left(left) {}
	};
public:
	// コンストラクタ
	PlayerState_Single(const Keys& keys);
	// 全てのステートに共通する初期化処理
	virtual void common_init(Actor& actor, const StateElement& element) override;
	// 入力処理
	virtual void input(float deltaTime) override;
	// 入力処理
	virtual void common_update(float deltaTime) override;
	// 衝突処理
	virtual void collide(const Actor & other) override;
	// 描画処理
	virtual void draw()const override;
protected:
	bool is_butty() const;
	bool is_retty() const;
protected:
	// プレイヤーポインタ
	PlayerBody* body_;
	// キー
	Keys keys_;
};