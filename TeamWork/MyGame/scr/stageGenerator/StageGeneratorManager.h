#pragma once
#include "StageGeneratorPtr.h"
#include "StageGenerator.h"
#include "Stage.h"
#include <unordered_map>

class StageGenerateManager
{
public:
	//�R���X�g���N�^
	StageGenerateManager();
	//�X�e�[�W�̒ǉ�
	void Add(const Stage name, const StageGeneratorPtr& stage);
	void SetStage(const Stage name);
	//�X�e�[�W�̏���
	void Clear();
	//�X�e�[�W�T�C�Y�擾
	Vector2 GetStageSize(const Stage name);

private:
	//�R�s�[�֎~
	StageGenerateManager(const StageGenerateManager& other) = delete;
	StageGenerateManager& operator = (const StageGenerateManager& other) = delete;

private:
	//�X�e�[�W
	std::unordered_map<Stage, StageGeneratorPtr> stages;
};