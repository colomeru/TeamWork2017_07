#pragma once

#include "../Base/PlayerState_HoldBase.h"

// プレイヤー片方くっつき状態
class PlayerState_HoldInit : public PlayerState_HoldBase {
public:
	// コンストラクタ
	PlayerState_HoldInit(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	// 状態固有の初期化
	virtual void onInit() override;
	// 更新処理	
	virtual void onUpdate(float deltaTime)override;
	// 終了時処理
	virtual void onEnd()override;
	// パッド入力処理
	virtual void onInput(float deltaTime)override;
	// 移動処理
	virtual void onMove(float deltaTime)override;
};