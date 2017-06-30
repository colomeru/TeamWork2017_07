#pragma once
#include "../../Actor.h"
#include "../../player/Player.h"
#include "../MyGame/scr/game/ID.h"
#include <array>
#include <vector>
#include <map>

enum CuttingState
{
	Normal,					//�ʏ�
	RightUpSlant,			//�E�΂ߏ�Ɍ������Đ؂�Ă�
	LeftUpSlant,			//���΂ߏ�Ɍ������Đ؂�Ă�
};


class Clothes : public Actor
{
protected:
	enum ClothesState
	{
		WINDLESS,				//���������Ă��Ȃ����
		BEGIN_WIND,				//���̐����n��
		BEGIN_STRONG_WIND,		//�������̐����n��
		STRONG_WIND,			//�������������Ă���
		ATTENUATE_WIND,			//���̈З͂̌���
		POSSIBLE_BITE,			//�v���C���[�����߂�悤�ɂȂ�
		END_WIND,				//�����I��
	};

	enum PendulumState
	{
		CENTER_LEFT,			//���S���獶�ɐU��Ƃ�
		LEFT_CENTER,			//�E���璆�S�ɐU��Ƃ�
		CENTER_RIGHT,			//���S����E�ɐU��Ƃ�
		RIGHT_CENTER,			//�E���璆�S�ɐU��Ƃ�
		NONE
	};

public:
	//�R���X�g���N�^
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight);
	//�f�X�g���N�^
	virtual ~Clothes() override;
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
	//�x�_�̎擾
	Vector2 GetFulcrum() const {
		return fulcrum_;
	}
	//�����蔻��̃|�C���g�̎擾
	std::vector<Vector2> GetCollisionPoints() const {
		return collisionPoints;
	}
	//�ؒf��Ԃ̎擾
	CuttingState GetCuttingState() {
		return cuttingState_;
	}
	//�摜�̃R�}�ԍ��̎擾
	int GetDrawFrame() const {
		return drawFrame_;
	}

	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

	//�U��q�^��(���p)
	void Pendulum(Vector2 fulcrum, float length);
	void Pendulum(std::vector<Vector2>& deformPos, float rot_spd, float length);
	//���ɂ�镞�h�炵
	void ShakesClothes();
	//�������ɂ��v���C���[�ւ̍�p
	void WindSwing();
	//���̓����蔻��̐ݒ�
	virtual void SetPointsUpdate();
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	virtual void SetLocalPoints();
	//���ɕt���������̕��̍X�V
	void UpdateClothesFeces();
	//���ɕt���������̕��̕`��
	void DrawClothesFeces() const;
	//
	void DrawRangeUpdate();
	//�⏕���`��
	void DrawRange() const;
	//�v���C���[�𕞂ɓ���������U��q
	void Synchronize();

	//���̗h���m���̐ݒ�
	void SetFrequencyWind(int wind);
	//���݂̃X�e�[�W�ԍ��̐ݒ�
	void SetCurrentStage(Stage currentStage);

	//�R�s�[�֎~
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;


private:
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̕ύX
	void SetNormal();
	void SetRightUpSlant();
	void SetLeftUpSlant();
	void PenStateDecision();

protected:
	//�Փ˂��Ă��邩
	bool isHit_;
	//��ID
	CLOTHES_ID clothes_ID;
	//�����蔻��̃|�C���g
	std::vector<Vector2> collisionPoints;
	//�����蔻��̃|�C���g�̃��[�J�����W
	std::vector<Vector3> localPoints;
	//�d��
	float weight_;
	//�摜�̃R�}�ԍ�
	int drawFrame_;
	//�������Ă��邩
	bool isDroping_;
	//���̏��
	ClothesState clothesState_;
	//���̐ؒf���
	CuttingState cuttingState_;
	//Delay�p�̒l
	float dNumber_;
	//���ɕt��������
	ActorPtr clothesFeces_;
	//�U��q�̏��
	PendulumState penState_;
	//�v���C���[
	Player* player_;
	//�U��q�̈ړ���
	Vector2 pendulumVec_;
	//�U��q�̈ړ��O�̈ʒu
	Vector2 beforePos_;
	//�����h���m��
	int frequencyWind_;
	//���݂̃X�e�[�W�ԍ�
	Stage currentStage_;
	//�⏕���`�����
	bool isDrawRange_;

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
	//���C
	float friction_;
	//�U��q�J�E���g
	int count_;
};