#pragma once
#include "../StageGenerator.h"
#include <queue>

class Stage1 : public StageGenerator
{
public:
	//�R���X�g���N�^
	Stage1(IWorld* world, std::string& fileName, int hairballCnt);
	//�f�X�g���N�^
	virtual ~Stage1() override;
	//�X�e�[�W�ǂݍ���
	virtual void LoadStage() override;
	//�X�e�[�W�ǉ�
	virtual void AddStage() override;

	//�X�e�[�W�T�C�Y�擾
	virtual Vector2 GetStageSize() const override {
		return stageSize_;
	}

private:
	void Pin_Add(int i, int j, int data, int laneNum);
	void Clothes_Add(int i, int j, int data, int laneNum);
	void GoalClothes_Add(int i, int j, int data, int laneNum);

private:
	std::queue<bool> pin_list;
	int hairballCnt_;
};