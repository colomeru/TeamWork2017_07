#pragma once

#include "../../Base/StateMgr.h"
#include "../../PlayerState_Enum.h"

#include "../../../PlayerPtr.h"

// ステートの処理管理
class PlayerStateMgr_Union : public StateMgr {
public:
	// コンストラクタ
	PlayerStateMgr_Union(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
	// 現在のステート
	bool get_state(const PlayerState_Enum_Union& id);
};