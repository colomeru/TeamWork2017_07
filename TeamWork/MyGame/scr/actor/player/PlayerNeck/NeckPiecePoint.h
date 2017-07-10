#pragma once
#include"../../../math/Vector2.h"

struct NeckPiecePoint {
	//先端の位置
	Vector2 tipPosLeft;
	Vector2 tipPosRight;
	//始点の位置
	Vector2 fulcrumLeft;
	Vector2 fulcrumRight;
};