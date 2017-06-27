#pragma once
#include "../StageGenerator.h"
#include "../../game/ID.h"
#include <queue>
#include <vector>
#include <map>

class Stage1 : public StageGenerator
{
public:
	//�R���X�g���N�^
	Stage1(IWorld* world, std::string& fileName);
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
	virtual void CreateClothes();
protected:
	//�s���ǉ��֐�
	void Pin_Add(int i, int j, int data, int laneNum);
	//���ǉ��֐�
	void Clothes_Add(int i, int j, int data, int laneNum);
	//�S�[���ǉ��֐��i�Q��ނ̃S�[���j
	void GoalClothes_Add(int i, int j, int data, int laneNum);

protected:
	std::queue<bool> pin_list;
	std::map<CLOTHES_ID, std::vector<SPRITE_ID>> spriteIdMap_;
};