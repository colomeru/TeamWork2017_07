#pragma once

#include "../Math/Vector2.h"

struct ScroolJudge {
	Vector2 scroolJudge;
	//このポジションをプレイヤーが超えたら止まる
	Vector2 scroolStopMax;
	Vector2 scroolStopMin;

};