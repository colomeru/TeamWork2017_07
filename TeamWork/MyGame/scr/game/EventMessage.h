#pragma once

enum class EventMessage
{
	BEGIN_WIND,			//風の吹き始め
	STRONG_WIND,		//強い風が吹く
	ATTENUATE_WIND,		//風の威力の減衰
	END_WIND,			//風が終了
	START_LANE_CHANGE,
	GOAL_FLAG,			//ゴールフラグ
	GAME_CLEAR_FLAG,	//ステージ終了フラグ
	TAPPER_DEAD,	//叩き死亡
	PLAY_NEXT_STAGE,
	ADD_SCORE,
	PLAYER_DEAD,
};