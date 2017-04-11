#pragma once

#include "../Base/PlayerState_HoldBase_Air.h"
#include "../Base/PlayerState_HoldBase_Begin.h"

// プレイヤー片方くっつき状態
class PlayerState_HoldAirBegin : public PlayerState_HoldBase_Air, public PlayerState_HoldBase_Begin {
public:
	// コンストラクタ
	PlayerState_HoldAirBegin(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	// 初期化
	virtual void onInit()override;
	// 更新処理	
	virtual void onUpdate(float deltaTime)override;
	// 終了時処理
	virtual void onEnd()override;
	// パッド入力処理
	virtual void onInput(float deltaTime)override;
	// 移動処理
	virtual void onMove(float deltaTime)override;
};