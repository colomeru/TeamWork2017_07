#pragma once
#include "../../../Def.h"
#include "../../../math/Vector2.h"
#include <array>
#include "../../../world/World.h"

class World;
class ProgressMeter
{
private:
	struct PinStruct
	{
		int lane;
		int posX;
	};
public:
	//�R���X�g���N�^
	ProgressMeter(){}
	//�R���X�g���N�^
	ProgressMeter(World* world, int stageLength);
	//�f�X�g���N�^
	~ProgressMeter();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw() const;
	//�I��
	void End();


private:
	World*		world_;

	int stageLen_; //�X�e�[�W�̒���
	int meterNum_; //���[�^�[�̖{��
	int meterLen_; //���[�^�[�̒���
	int nowLane_; //�v���C���[�̂��郌�[���ԍ�
	Vector2 meterPos_; //���[�^�[���W
	Vector2 pIconPos_; //�v���C���[�A�C�R���̍��W
	float pPosY_; //�v���C���[�A�C�R����y���W
	int prevLane_; //1�t���[���O�̃��[���ԍ�
	int dis_; //���[�^�[�ԋ���

	//�摜�̃n���h��
	int pinHandle_; //�s��
	int pIconHandle_; //�v���C���[�A�C�R��
	int laneHandle_; //���[��
	int startHandle_; //�X�^�[�g
	int goalHandle_; //�S�[��

	//�摜�̃T�C�Y
	Vector2 pinSize_; //�s��
	Vector2 pIconSize_; //�v���C���[�A�C�R��
	Vector2 laneSize_; //���[���P
	Vector2 startSize_; //�X�^�[�g
	Vector2 goalSize_; //�S�[��

};