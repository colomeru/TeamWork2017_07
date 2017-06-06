#pragma once
#include "../../../Def.h"
#include "../../../math/Vector2.h"
#include <array>
#include "../../../world/World.h"

class World;
class ProgressMeter
{
public:
	//�R���X�g���N�^
	ProgressMeter(){}
	//�R���X�g���N�^
	ProgressMeter(World* world, int stageLength, int pinPosition);
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
	////��i
	//void UpperLane();
	//void UpperDraw() const;
	////���i
	//void MiddleLane();
	//void MiddleDraw() const;
	////���i
	//void LowerLane();
	//void LowerDraw();	


private:
	World*		world_;

	int stageLen_; //�X�e�[�W�̒���
	int pinPos_; //�s���̍��W
	int meterNum_; //���[�^�[�̖{��
	int meterLen_; //���[�^�[�̒���
	int nowLane_; //�v���C���[�̂��郌�[���ԍ�
	Vector2 meterPos_; //���[�^�[���W
	Vector2 pIconPos_; //�v���C���[�A�C�R���̍��W

	//�摜�̃n���h��
	int pinHandle_; //�s��
	int playerHandle_; //�J�[�\��
	int upperHandle_; //���[���P�i��i�j
	int middleHandle_; //���[���Q�i���i�j
	int lowerHandle_; //���[���R�i���i�j
	int startHandle_; //�X�^�[�g
	int goalhandle_; //�S�[��

};