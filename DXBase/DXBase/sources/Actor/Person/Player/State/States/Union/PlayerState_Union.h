#pragma once

#include "../../Base/State.h"

#include "../../PlayerState_Enum.h"

#include "../../../PlayerPtr.h"
#include "../../../Player.h"
#include "../../../PlayerConnector.h"
#include "../../../PlayerBody.h"

// 各プレイヤーステートの基底クラス
class PlayerState_Union : public State {
public:
	// コンストラクタ
	PlayerState_Union(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
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
	// ステートの変更処理
	void change(const PlayerState_Enum_Union& id, const ActionType& type = ActionType::None);
protected:
	// パッド系フラグ
	bool holdable_padR();
	bool holdable_padL();
	bool move_padR();
	bool move_padL();
	bool jump_pad();
	// スライムの弾性表現
	Vector2 speed_b(float deltaTime);
	Vector2 speed_r(float deltaTime);
	void chase(Vector2 &position, int pointIndex);
	Vector2 clamp(const Vector2 &position, int pointIndex);

	bool isOnFloor();
protected:
	// ばね補正速度
	Vector2 velocity_;
	// プレイヤーポインタ
	PlayerConnector* cntr_;
	// プレイヤーポインタ
	PlayerBodyPtr butty_;
	// プレイヤーポインタ
	PlayerBodyPtr retty_;
};