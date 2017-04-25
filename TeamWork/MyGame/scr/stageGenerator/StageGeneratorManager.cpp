#include "StageGeneratorManager.h"

//コンストラクタ
StageGenerateManager::StageGenerateManager()
{
}

//ステージの追加
void StageGenerateManager::Add(Stage name, const StageGeneratorPtr & stage)
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

//ステージサイズ取得
Vector2 StageGenerateManager::GetStageSize(const Stage name)
{
	return stages[name]->GetStageSize();
}

bool StageGenerateManager::GetGoalFlag(const Stage name)
{
	return stages[name]->GetGoalFlag();
}
