#pragma once
#include "../game/ID.h"
#include "../math/Vector2.h"
#include "../math/Matrix.h"
#include<vector>
struct ActorParameter
{
	ACTOR_ID		ID;			// �A�N�^�[ID
	bool			isDead;		// ���S�t���O
	float			radius;		// ���a
	float			HP;			// �̗�
	Vector2			size;		// �T�C�Y
	Matrix			mat;		// �}�g���b�N�X
	float			spriteAlpha_;//�s�����x(0.f~1.f)
	

	//�����`��p�̓_�ꗗ(pos����e�_�ǂ̒��x����Ă��邩�����)����o����悤�ɓ_���w�肷�鎖
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