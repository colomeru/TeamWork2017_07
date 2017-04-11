#pragma once

#include "Base/PlayerState_HoldBase_Wall.h"

class PlayerState_HoldItem : public PlayerState_HoldBase_Wall {
public:
	// コンストラクタ
	PlayerState_HoldItem(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	// 状態固有の初期化
	virtual void onInit() override;
	// 更新処理	
	virtual void onUpdate(float deltaTime)override;
	// 終了時処理
	virtual void onEnd()override;
	// パッド入力処理
	virtual void onInput(float deltaTime)override;
};