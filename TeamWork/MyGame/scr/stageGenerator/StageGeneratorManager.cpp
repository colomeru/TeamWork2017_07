#include "StageGeneratorManager.h"

//�R���X�g���N�^
StageGenerateManager::StageGenerateManager()
{
}

//�X�e�[�W�̒ǉ�
void StageGenerateManager::Add(Stage name, const StageGeneratorPtr & stage)
{
	stages[name] = stage;
}

void StageGenerateManager::SetStage(Stage name)
{
	auto currentStage = stages[name];
	currentStage->LoadStage();
}

//�X�e�[�W�̏���
void StageGenerateManager::Clear()
{
	stages.clear();
}

//�X�e�[�W�T�C�Y�擾
Vector2 StageGenerateManager::GetStageSize(const Stage name)
{
	return stages[name]->GetStageSize();
}

bool StageGenerateManager::GetGoalFlag(const Stage name)
{
	return stages[name]->GetGoalFlag();
}
