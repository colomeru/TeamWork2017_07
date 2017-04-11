#pragma once

#include "../Base/PlayerState_HoldBase.h"

// プレイヤー伸び始めの状態
class PlayerState_HoldBase_Begin : public virtual PlayerState_HoldBase {
public:
	// コンストラクタ
	PlayerState_HoldBase_Begin(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
protected:
	// 状態固有の初期化
	virtual void onInit() override;
	// 終了時処理
	virtual void onEnd()override;
	// パッド入力処理
	virtual void onInput(float deltaTime)override;
};