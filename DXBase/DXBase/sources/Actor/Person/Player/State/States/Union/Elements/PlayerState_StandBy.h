#pragma once

#include "../PlayerState_Union.h"

// 準備状態
class PlayerState_StandBy : public PlayerState_Union {
public:
	// コンストラクタ
	PlayerState_StandBy(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	// 状態固有の初期化
	virtual void unique_init() override;
	// 更新処理	
	virtual void update(float deltaTime) override {}
	// 終了時処理
	virtual void end() override {}
};