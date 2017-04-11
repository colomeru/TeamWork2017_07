#pragma once

#include "../../Base/StateMgr.h"

// 先行宣言
enum class PlayerState_Enum_Single;

// 分離時のステート管理
class PlayerStateMgr_Single : public StateMgr {
public:
	// コンストラクタ
	PlayerStateMgr_Single(const std::string& name = "");
	// 現在のステート
	bool get_state(const PlayerState_Enum_Single& id);
};