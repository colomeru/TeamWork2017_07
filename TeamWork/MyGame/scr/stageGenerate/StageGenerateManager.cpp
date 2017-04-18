#include "StageGenerateManager.h"

//�R���X�g���N�^
StageGenerateManager::StageGenerateManager()
{
}

//�X�e�[�W�̒ǉ�
void StageGenerateManager::Add(Stage name, const IStageGeneratePtr & stage)
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
