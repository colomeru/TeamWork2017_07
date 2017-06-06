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
	int playerHandle_; //�J�[�\��
	int upperHandle_; //���[���P�i��i�j
	int middleHandle_; //���[���Q�i���i�j
	int lowerHandle_; //���[���R�i���i�j
	int startHandle_; //�X�^�[�g
	int goalhandle_; //�S�[��

};