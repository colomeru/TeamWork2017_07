#pragma once
#include "../StageGenerator.h"
#include "../Stage.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/PointSetter/PointSetter.h"
#include <queue>
#include <vector>
#include <map>

class Stage1 : public StageGenerator
{
public:
	//�R���X�g���N�^
	Stage1(IWorld* world, std::string& fileName, int frequencyWind = 0, int frequencyHairball = 0, int hairballCnt = 0);
	//�f�X�g���N�^
	virtual ~Stage1() override;
	//�X�e�[�W�ǂݍ���
	virtual void LoadStage(Stage name = Stage::Stage1) override;
	//�X�e�[�W�ǉ�
	virtual void AddStage() override;

	//�X�e�[�W�T�C�Y�擾
	virtual Vector2 GetStageSize() const override {
		return stageSize_;
	}
	//�`���[�g���A���p�̕�����
	void CreateClothes();
protected:
	//�s���ǉ��֐�
	void Pin_Add(int i, int j, int data, int laneNum);
	//���ǉ��֐�
	void Clothes_Add(int i, int j, int data, int laneNum);

protected:
	//�s���������ǂ���
	std::queue<bool> pin_list;
	//�����ꂼ��̃X�v���C�g�̃}�b�v
	std::map<CLOTHES_ID, std::vector<SPRITE_ID>> spriteIdMap_;
	//�����h���m��
	int frequencyWind_;
	//�ыʂ����ł���m��
	int frequencyHairball_;
	//�ыʂ̔�ԕp�x
	int hairballCnt_;
	//���݂̃X�e�[�W
	Stage currentStage_;
	//���̃|�C���g�ݒ�W
	PointSetter pointSetter_;
};