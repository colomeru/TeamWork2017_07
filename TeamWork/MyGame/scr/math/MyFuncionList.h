#pragma once
#include"../math/Vector2.h"
#include"../graphic/DrawPos.h"
#include"../math/Matrix.h"
#include"../math/Vector3.h"
#include"../math/MathHelper.h"
#include<vector>

template<typename _Tp> inline int sign(_Tp val) {
	return 1 - (val <= 0) - (val < 0);
}

//rollVect=回転方向(1で右回り)
inline float MathAngle(const Vector2& targetVect,const Vector2& baseVect=Vector2::Up,int rollVect=1) {
	Vector2 nTVect = targetVect;
	Vector2 nBVect = baseVect;

	float angle = Vector2::Dot(nTVect, nBVect) / (nTVect.Length()*nBVect.Length());
	angle = MathHelper::ToDegrees(acos(angle));
	if (sign(nTVect.x) > 0) {
		angle = 360 - angle;

	}
	return angle;
}
//指定軸とのなす角の角度を返す
inline float MathFormedAngle(const Vector2& targetVect, const Vector2& baseVect = Vector2::Up) {
	Vector2 nTVect = targetVect;
	Vector2 nBVect = baseVect;
	
	float angle = Vector2::Dot(nTVect, nBVect) / (nTVect.Length()*nBVect.Length());
	angle = MathHelper::ToDegrees(acos(angle));

	return angle;
}

//描画開始点と、描画向き、横幅、縦幅から、描画範囲を表す4点を返す 描画位置dirN=方向
inline DrawPos MathDrawPoint(const Vector2& basePos,const Vector2& dir, int width,int height) {
	DrawPos p;

	//頭から体へのベクトル(正規化)
	Vector2 pHtoBVec2 = Vector2::Normalize(dir);
	Vector3 pHtoBVec3 = pHtoBVec2;

	//HtoBベクトルを90度回転
	Vector3 pHtBRotateVec = pHtoBVec3*Matrix::CreateRotationZ(90);
	//Vec2に変換
	Vector2 pHtBNVerticalVec(pHtBRotateVec.x, pHtBRotateVec.y);
	pHtBNVerticalVec = pHtBNVerticalVec.Normalize();

	Vector2 drawBasePos = basePos;

	p.p0 = drawBasePos - (pHtBNVerticalVec*(float)width);
	p.p1 = drawBasePos + (pHtBNVerticalVec*(float)width);

	drawBasePos += pHtoBVec2*(float)height;

	p.p2 = drawBasePos + (pHtBNVerticalVec*(float)width);
	p.p3 = drawBasePos - (pHtBNVerticalVec*(float)width);

	return p;
}

inline int DigitLength(int num) {
	if (num == 0) {
		return 1;
	}

	int count = 0;
	while (num) {
		num = num / 10;
		++count;
	}
	return count;
}

inline std::vector<int> SligeDigit(int num) {
	int mod = 0;
	std::vector<int> result;

	if (num == 0) {
		result.push_back(0);
		return result;
	}

	int i = 0;
	while (num >= 1) {
		mod = num % 10;
		num = num / 10;
		result.push_back(mod);
	}
	return result;

}