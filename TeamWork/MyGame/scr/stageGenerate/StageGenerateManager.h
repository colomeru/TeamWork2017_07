#pragma once

#include "IStageGeneratePtr.h"
#include "IStageGenerate.h"
#include "Stage.h"
#include <unordered_map>

class StageGenerateManager
{
public:
	//�R���X�g���N�^
	StageGenerateManager();
	//�X�e�[�W�̒ǉ�
	void Add(Stage name, const IStageGeneratePtr& stage);
	void SetStage(Stage name);
	//�X�e�[�W�̏���
	void Clear();

private:
	//�R�s�[�֎~
	StageGenerateManager(const StageGenerateManager& other) = default;
	StageGenerateManager& operator = (const StageGenerateManager& other) = default;

private:
	//�X�e�[�W
	std::unordered_map<Stage, IStageGeneratePtr> stages;
};