#pragma once

#include "../../../PlayerState_Union.h"

// プレイヤーくっつきベース
class PlayerState_HoldBase : public PlayerState_Union {
public:
	// コンストラクタ
	PlayerState_HoldBase(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	// 状態固有の初期化
	virtual void unique_init() override;
	// 更新処理	
	virtual void update(float deltaTime) override;
	// 終了時処理
	virtual void end() override;
	// 入力処理
	virtual void input(float deltaTime) override;
private:
	// 移動処理
	void move(float deltaTime);
private:
	// 移動速度計算
	Vector2 input_vector(const Vector2& input, float deltaTime);
protected:
	// くっつきボタンが離れたかどうか
	bool hold_released();
private:
	// 初期化
	virtual void onInit() = 0;
	// 更新処理	
	virtual void onUpdate(float deltaTime) = 0;
	// 終了時処理
	virtual void onEnd() = 0;
	// パッド入力処理
	virtual void onInput(float deltaTime) = 0;
	// 移動処理
	virtual void onMove(float deltaTime) = 0;
protected:
	// プレイヤーポインタ(くっついている)
	PlayerBodyPtr holdBody_;
	// プレイヤーポインタ(移動可能なBody)
	PlayerBodyPtr moveBody_;
};