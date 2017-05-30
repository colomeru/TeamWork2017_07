#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"
#include "../../player/Player_Head.h"
#include <array>
#include <vector>

class Clothes : public Actor
{
protected:
	enum ClothesState
	{
		BEGIN_WIND,				//���̐����n��
		BEGIN_STRONG_WIND,		//�������̐����n��
		STRONG_WIND,			//�������������Ă���
		ATTENUATE_WIND,			//���̈З͂̌���
		POSSIBLE_BITE,			//�v���C���[�����߂�悤�ɂȂ�
		END_WIND,				//�����I��
	};

	enum ClothesCuttingState
	{
		Normal,					//�ʏ�
		RightUpSlant,			//�E�΂ߏ�Ɍ������Đ؂�Ă�
		LeftUpSlant,			//���΂ߏ�Ɍ������Đ؂�Ă�
		HorizontalSlant			//�����ɐ؂�Ă�
	};

public:
	//�R���X�g���N�^
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum);
	//�f�X�g���N�^
	virtual ~Clothes();
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() const = 0;

	//ID�̎擾
	CLOTHES_ID GetClothesID() const {
		return clothes_ID;
	}
	//�����󂯂Ă��邩�̎擾
	bool GetIsWind() const {
		return isWind_;
	}
	Vector2 GetFulcrum() const {
		return fulcrum_;
	}
	std::vector<Vector2> GetCollisionPoints() const {
		return collisionPoints;
	}

	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

	//�U��q�^��(���p)
	void Pendulum(Vector2 fulcrum, float length);
	//���ɂ�镞�h�炵
	void ShakesClothes();
	//�������ɂ��v���C���[�ւ̍�p
	void WindSwing();
	//���̓����蔻��̐ݒ�
	void SetPointsUpdate();
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	void SetLocalPoints();

	//�R�s�[�֎~
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;


public:


protected:
	//�Փ˂��Ă��邩
	bool isHit_;
	//�e�X�g
	bool is_Test_;
	//��ID
	CLOTHES_ID clothes_ID;
	//�����蔻��̃|�C���g
	std::vector<Vector2> collisionPoints;
	//�����蔻��̃|�C���g�̃��[�J�����W
	std::vector<Vector3> localPoints;

	//�e�X�g
	Vector2 intersectPos_;


	//�U��q�֘A(���p)
	//�U��q�t���O
	bool isPendulum_;
	//���C��������������
	bool isFriction_;
	//�����󂯂Ă��邩
	bool isWind_;
	//�x�_���W
	Vector2 fulcrum_;
	//�p�x
	float rot_;
	//�p���x
	float rot_spd_;
	//�R�̒���
	float length_;
	//�d�͉����x
	float gravity_;
	//�U��q�O�̌Œ�ʒu
	Vector2 basePosition_;
	//���C
	float friction_;
	//�U��q�J�E���g
	int count_;
	//���̏��
	ClothesState clothesState_;
	//���̐ؒf���
	ClothesCuttingState cuttingState_;
};