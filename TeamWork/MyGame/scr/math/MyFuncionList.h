#pragma once
#include"../math/Vector2.h"

template<typename _Tp> inline int sign(_Tp val) {
	return 1 - (val <= 0) - (val < 0);
}

//rollVect=‰ñ“]•ûŒü(1‚Å‰E‰ñ‚è)
inline float MathAngle(const Vector2& targetVect,const Vector2& baseVect=Vector2::Up,int rollVect=1) {
	//Vector2 nTVect = Vector2::Normalize(targetVect);
	//Vector2 nBVect = Vector2::Normalize(baseVect);
	Vector2 nTVect = targetVect;
	Vector2 nBVect = baseVect;

	float angle = Vector2::Dot(nTVect, nBVect) / (nTVect.Length()*nBVect.Length());
	angle = MathHelper::ToDegrees(acos(angle));
	if (sign(nTVect.x) > 0) {
		angle = 360 - angle;

	}


	return angle;
}