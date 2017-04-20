#pragma once
#include "../StageGenerator.h"

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
	virtual Vector2 GetStageSize() const override;
};