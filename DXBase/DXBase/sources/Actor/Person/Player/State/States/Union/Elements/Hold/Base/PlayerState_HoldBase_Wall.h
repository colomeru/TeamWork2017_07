#pragma once

#include "../Base/PlayerState_HoldBase.h"

// プレイヤー壁くっつき状態ベース
class PlayerState_HoldBase_Wall : public virtual PlayerState_HoldBase{
public:
	// コンストラクタ
	PlayerState_HoldBase_Wall(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
protected:
	// 更新処理	
	virtual void onUpdate(float deltaTime)override;
	// パッド入力処理
	virtual void onInput(float deltaTime)override;
	// 移動処理
	virtual void onMove(float deltaTime)override;
};