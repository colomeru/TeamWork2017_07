#include "PlayerState_StandBy.h"

// コンストラクタ
PlayerState_StandBy::PlayerState_StandBy(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) : PlayerState_Union(butty, retty) {}

// 状態固有の初期化
void PlayerState_StandBy::unique_init(){
	// 各プレイヤーの状態を準備状態に変更
	butty_->change_state(PlayerState_Enum_Single::STAND_BY);
	retty_->change_state(PlayerState_Enum_Single::STAND_BY);

	// 待機状態に遷移
	change(PlayerState_Enum_Union::IDLE);
}


