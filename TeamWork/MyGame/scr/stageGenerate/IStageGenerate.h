#pragma once
#include "../world/IWorld.h"
#include "../input/CsvReader.h"

class IStageGenerate
{
public:
	//�R���X�g���N�^
	IStageGenerate(IWorld* world, const std::string& fileName);
	//���z�f�X�g���N�^
	virtual ~IStageGenerate();
	//�X�e�[�W�ǂݍ���
	virtual void LoadStage() = 0;
	//�X�e�[�W�ǉ�
	virtual void AddStage() = 0;

private:
	//�R�s�[�֎~
	IStageGenerate(const IStageGenerate& other) = default;
	IStageGenerate& operator = (const IStageGenerate& other) = default;

protected:
	//���[���h
	IWorld* world_;
	//���[�_�[
	CsvReader csvReader_;
	//
	std::string fileName_;

};

