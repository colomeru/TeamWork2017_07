#pragma once
#include "../game/ID.h"
#include "../math/Vector2.h"

struct CollisionParameter
{
	COL_ID		colID;
	//ìñÇΩÇ¡ÇΩÇ©
	bool		colFrag = false;
	//ê⁄ì_
	Vector2		colPos  = Vector2::Zero;

	CollisionParameter(COL_ID id = COL_ID::BEGIN_COL, bool frag = false, const Vector2& position = Vector2::Zero) :
	colID(id), colFrag(frag), colPos(position)
	{
	}
};