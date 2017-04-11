#pragma once

#include "../Base/PlayerState_HoldBase.h"

// プレイヤー空中でのくっつき状態ベース
class PlayerState_HoldBase_Air : public virtual PlayerState_HoldBase {
public:
	// コンストラクタ
	PlayerState_HoldBase_Air(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
protected:
	// 更新処理	
	virtual void onUpdate(float deltaTime)override;
	// 移動処理
	virtual void onMove(float deltaTime)override;
};
