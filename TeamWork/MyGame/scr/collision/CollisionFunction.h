#pragma once

class IWorld;
class Actor;
struct CollisionParameter;

class CollisionFunction
{
public:
	// �R���X�g���N�^
	explicit CollisionFunction(IWorld* world); 

	// �X�v���C�g�̓����蔻��i�Q������OBB vs OBB�j
	CollisionParameter IsHit_OBB_OBB(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q������Player�̓� vs ���j
	CollisionParameter IsHit_PHead_Clothes(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q������Player�̓� vs �n���K�[�j
	CollisionParameter IsHit_PHead_Hanger(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q������OBB vs �����j
	CollisionParameter IsHit_OBB_Segment(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q�����̐��� vs �����j
	CollisionParameter IsHit_Segment_Segment(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q�����̉~ vs �~�j
	CollisionParameter IsHit_Circle_Circle(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q�����̉~ vs �����j
	CollisionParameter IsHit_Circle_Segment(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q������OBB vs �~�j
	CollisionParameter IsHit_OBB_Circle(const Actor& sprite1, const Actor& sprite2);
	// �X�v���C�g�̓����蔻��i�Q������OBB vs ���j
	CollisionParameter IsHit_OBB_Clothes(const Actor& sprite1, const Actor& sprite2);
	CollisionParameter IsHit_Clothes_OBB(const Actor& sprite1, const Actor& sprite2);


private:
	// ���[���h
	IWorld*	world_;
};