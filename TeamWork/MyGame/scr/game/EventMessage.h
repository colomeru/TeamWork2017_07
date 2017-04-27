#pragma once

enum class EventMessage
{
	BEGIN_WIND,			//風の吹き始め
	STRONG_WIND,		//強い風が吹く
	ATTENUATE_WIND,		//風の威力の減衰
	END_WIND,			//風が終了

	GOAL_FLAG,			//ゴールフラグ
};