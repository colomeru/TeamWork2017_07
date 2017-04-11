#pragma once

// プレイヤーのステート一覧
enum class PlayerState_Enum_Union {
	DAMMY,
	STAND_BY,
	IDLE,
	MOVE,
	MOVE_BOTH,
	HOLD,
	HOLD_BEGIN,
	HOLD_SWIM,
	HOLD_FULL,
	HOLD_AIR,
	HOLD_AIR_SWIM,
	HOLD_AIR_FULL,
	HOLD_BOTH,
	HOLD_ITEM,
	JUMP,
	ATTACK,
	//ATTACK_R,
	//ATTACK_L,
	FREEZE,
	EVENT,
	LEAN_BACK
};

// プレイヤーのステート一覧
enum class PlayerState_Enum_Single {
	DAMMY,
	STAND_BY,
	IDLE,
	DEAD,
	LEAN_BACK
};