#pragma once
#include "../../Actor.h"
#include "../../player/Player.h"
#include "../MyGame/scr/game/ID.h"
#include "../../../debugdata/DebugDraw.h"

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

public:
	//�R���X�g���N�^
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight, 
		std::map<CuttingState, std::vector<Vector3>> localPoints = std::map<CuttingState, std::vector<Vector3>>());
	//�f�X�g���N�^
	virtual ~Clothes() override;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() const = 0;
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;
	//���̓����蔻��̐ݒ�
	virtual void SetPointsUpdate();

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
	//�U��q�^��(���p)
	void Pendulum(Vector2 fulcrum, float length);
	//���ɂ�镞�h�炵
	void ShakesClothes();
	//�������ɂ��v���C���[�ւ̍�p
	void WindSwing();
	//���ɕt���������̕��̍X�V
	void UpdateClothesFeces();
	//���ɕt���������̕��̕`��
	void DrawClothesFeces() const;
	//�⏕���`������邩�ǂ���
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

protected:
	//��ID
	CLOTHES_ID clothes_ID;
	//�����蔻��̃|�C���g
	std::vector<Vector2> collisionPoints;
	//�����蔻��̃|�C���g�̃��[�J�����W
	std::vector<Vector3> localPoints;
	std::map<CuttingState, std::vector<Vector3>> localPoints_;
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
	//�R�̒���
	const float LENGTH = 125.0f;
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
	//���C
	float friction_;
	//�U��q�J�E���g
	int count_;
};