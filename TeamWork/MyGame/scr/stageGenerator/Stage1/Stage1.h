#pragma once
#include "../StageGenerator.h"
#include <queue>

class Stage1 : public StageGenerator
{
public:
	//コンストラクタ
	Stage1(IWorld* world, std::string& fileName, int hairballCnt);
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

private:
	//ピン追加関数
	void Pin_Add(int i, int j, int data, int laneNum);
	//服追加関数
	void Clothes_Add(int i, int j, int data, int laneNum);
	//ゴール追加関数（２種類のゴール）
	void GoalClothes_Add(int i, int j, int data, int laneNum);

private:
	std::queue<bool> pin_list;
	int hairballCnt_;
};