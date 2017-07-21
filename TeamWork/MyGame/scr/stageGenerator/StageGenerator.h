#pragma once
#include "../world/IWorld.h"
#include "../input/CsvReader.h"
#include "Stage.h"

class StageGenerator
{
public:
	//�R���X�g���N�^
	StageGenerator(IWorld* world, const std::string& fileName);
	//���z�f�X�g���N�^
	virtual ~StageGenerator() = default;
	//�X�e�[�W�ǂݍ���
	virtual void LoadStage(Stage name = Stage::Stage1) = 0;
	//�X�e�[�W�ǉ�
	virtual void AddStage() = 0;
	//�X�e�[�W�T�C�Y�擾
	virtual Vector2 GetStageSize() const = 0;

private:
	//�R�s�[�֎~
	StageGenerator(const StageGenerator& other) = delete;
	StageGenerator& operator = (const StageGenerator& other) = delete;

protected:
	//���[���h
	IWorld* world_;
	//���[�_�[
	CsvReader csvReader_;
	//csv�t�@�C����
	std::string fileName_;
	//�X�e�[�W�T�C�Y
	Vector2 stageSize_;
	//�X�e�[�W��1�}�X�̃T�C�Y
	const int STAGE_TIP_SIZE;
};

