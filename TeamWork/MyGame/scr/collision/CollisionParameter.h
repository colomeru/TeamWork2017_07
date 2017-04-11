#pragma once
#include "../game/ID.h"
#include "../math/Vector3.h"

struct CollisionParameter
{
	COL_ID		colID;
	bool		colFrag = false;
	Vector3		colPos  = Vector3::Zero;

	CollisionParameter(COL_ID id = COL_ID::BEGIN_COL, bool frag = false, const Vector3& position = Vector3::Zero) :
	colID(id), colFrag(frag), colPos(position)
	{
	}
};