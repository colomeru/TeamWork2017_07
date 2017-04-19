#pragma once

#include "IStageGeneratePtr.h"
#include "IStageGenerate.h"
#include "Stage.h"
#include <unordered_map>

class StageGenerateManager
{
public:
	//コンストラクタ
	StageGenerateManager();
	//ステージの追加
	void Add(Stage name, const IStageGeneratePtr& stage);
	void SetStage(Stage name);
	//ステージの消去
	void Clear();

private:
	//コピー禁止
	StageGenerateManager(const StageGenerateManager& other) = default;
	StageGenerateManager& operator = (const StageGenerateManager& other) = default;

private:
	//ステージ
	std::unordered_map<Stage, IStageGeneratePtr> stages;
};