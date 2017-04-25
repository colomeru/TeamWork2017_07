#pragma once
#include "../StageGenerator.h"
#include "../MyGame/scr/actor/Field/Clothes/GoalClothes/GoalClothes.h"

class Stage1 : public StageGenerator
{
public:
	//コンストラクタ
	Stage1(IWorld* world, std::string& fileName);
	//デストラクタ
	virtual ~Stage1() override;
	//ステージ読み込み
	virtual void LoadStage() override;
	//ステージ追加
	virtual void AddStage() override;

	//ステージサイズ取得
	virtual Vector2 GetStageSize() const override {
		return stageSize_;
	}
	//ゴールフラグ取得
	virtual bool GetGoalFlag() const override {
		return goal_->GetGoalFlag();
	}

private:
	using GoalPtr = std::shared_ptr<GoalClothes>;
	GoalPtr			goal_;
};