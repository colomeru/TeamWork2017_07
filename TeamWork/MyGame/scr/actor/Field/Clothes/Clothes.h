#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"

class Clothes : public Actor
{
public:
	//�R���X�g���N�^
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum);
	//���z�f�X�g���N�^
	virtual ~Clothes() = default;
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

	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

	//�U��q�^��(���p)
	void Pendulum(Vector2 fulcrum, float length);
	//���ɂ�镞�h�炵
	void ShakesClothes();

	//�R�s�[�֎~
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;

protected:
	//�Փ˂��Ă��邩
	bool isHit_;
	//��ID
	CLOTHES_ID clothes_ID;

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
};