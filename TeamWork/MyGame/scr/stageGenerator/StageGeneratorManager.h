#pragma once
#include "StageGeneratorPtr.h"
#include "StageGenerator.h"
#include "Stage.h"
#include <unordered_map>

class StageGenerateManager
{
public:
	//コンストラクタ
	StageGenerateManager();
	//ステージの追加
	void Add(const Stage name, const StageGeneratorPtr& stage);
	void SetStage(const Stage name);
	//ステージの消去
	void Clear();
	//ステージサイズ取得
	Vector2 GetStageSize(const Stage name);

private:
	//コピー禁止
	StageGenerateManager(const StageGenerateManager& other) = default;
	StageGenerateManager& operator = (const StageGenerateManager& other) = default;

private:
	//ステージ
	std::unordered_map<Stage, StageGeneratorPtr> stages;
};