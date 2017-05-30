#pragma once
#include"../math/Vector2.h"
#include"../graphic/DrawPos.h"
#include"../math/Matrix.h"

template<typename _Tp> inline int sign(_Tp val) {
	return 1 - (val <= 0) - (val < 0);
}

//rollVect=��]����(1�ŉE���)
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
//�`��J�n�_�ƁA�`������A�����A�c������A�`��͈͂�\��4�_��Ԃ� �`��ʒudirN=����
inline DrawPos MathDrawPoint(const Vector2& basePos,const Vector2& dir, int width,int height) {
	DrawPos p;

	//������̂ւ̃x�N�g��(���K��)
	Vector2 pHtoBVec2 = Vector2::Normalize(dir);
	Vector3 pHtoBVec3 = pHtoBVec2;

	//HtoB�x�N�g����90�x��]
	Vector3 pHtBRotateVec = pHtoBVec3*Matrix::CreateRotationZ(90);
	//Vec2�ɕϊ�
	Vector2 pHtBNVerticalVec(pHtBRotateVec.x, pHtBRotateVec.y);
	pHtBNVerticalVec = pHtBNVerticalVec.Normalize();

	Vector2 drawBasePos = basePos;

	p.p0 = drawBasePos - (pHtBNVerticalVec*width);
	p.p1 = drawBasePos + (pHtBNVerticalVec*width);

	drawBasePos += pHtoBVec2*height;

	p.p2 = drawBasePos + (pHtBNVerticalVec*width);
	p.p3 = drawBasePos - (pHtBNVerticalVec*width);

	return p;
}