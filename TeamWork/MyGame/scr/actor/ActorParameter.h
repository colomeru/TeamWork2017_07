#pragma once
#include "../game/ID.h"
#include "../math/Vector2.h"
#include "../math/Matrix.h"
#include<vector>
struct ActorParameter
{
	ACTOR_ID		ID;			// アクターID
	bool			isDead;		// 死亡フラグ
	float			radius;		// 半径
	float			HP;			// 体力
	Vector2			size;		// サイズ
	Matrix			mat;		// マトリックス
	float			spriteAlpha_;//不透明度(0.f~1.f)
	

	//線分描画用の点一覧(posから各点どの程度離れているかを入力)周回出来るように点を指定する事
	std::vector<Vector2> ClothSegmentPoints_;

	ActorParameter(ACTOR_ID id = ACTOR_ID::BEGIN_ACTOR, bool isDead = false, float radius = 0.0f, float hp = 0.0f, const Vector2& size = Vector2::Zero, const Matrix& mat = Matrix::Identity,float spriteAlpha=1.0f) :
		ID(id), isDead(isDead), radius(radius), HP(hp), size(size), mat(mat), spriteAlpha_(spriteAlpha)
	{
	}
};

struct SymbolParameter
{
	ACTOR_ID		listID;
	ACTOR_ID		ID;
	Matrix			mat;
	bool			isEncount;
	bool			isDead;

	SymbolParameter(ACTOR_ID listId = ACTOR_ID::BEGIN_ACTOR, ACTOR_ID id = ACTOR_ID::BEGIN_ACTOR, const Matrix& mat = Matrix::Identity, bool encount = false, bool dead = false) :
		listID(listId), ID(id), mat(mat), isEncount(encount), isDead(dead)
	{
	}
};