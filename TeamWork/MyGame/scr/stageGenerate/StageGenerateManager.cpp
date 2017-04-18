#include "StageGenerateManager.h"

//コンストラクタ
StageGenerateManager::StageGenerateManager()
{
}

//ステージの追加
void StageGenerateManager::Add(Stage name, const IStageGeneratePtr & stage)
{
	stages[name] = stage;
}

void StageGenerateManager::SetStage(Stage name)
{
	auto currentStage = stages[name];
	currentStage->LoadStage();
}

//ステージの消去
void StageGenerateManager::Clear()
{
	stages.clear();
}
