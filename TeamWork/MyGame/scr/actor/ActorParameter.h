#pragma once
#include "../game/ID.h"
#include "../math/Vector2.h"
#include "../math/Matrix.h"

struct ActorParameter
{
	ACTOR_ID		ID;			// アクターID
	bool			isDead;		// 死亡フラグ
	float			radius;		// 半径
	float			HP;			// 体力
	Vector2			size;		// サイズ
	Matrix			mat;		// マトリックス
	

	ActorParameter(ACTOR_ID id = ACTOR_ID::BEGIN_ACTOR, bool isDead = false, float radius = 0.0f, float hp = 0.0f, const Vector2& size = Vector2::Zero, const Matrix& mat = Matrix::Identity) :
		ID(id), isDead(isDead), radius(radius), HP(hp), size(size), mat(mat)
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