#pragma once
#include "../StageGenerator.h"
#include "../Stage.h"
#include "../../game/ID.h"
#include <queue>
#include <vector>
#include <map>

class Stage1 : public StageGenerator
{
public:
	//コンストラクタ
	Stage1(IWorld* world, std::string& fileName, int frequencyWind = 0, int frequencyHairball = 0, int hairballCnt = 0);
	//デストラクタ
	virtual ~Stage1() override;
	//ステージ読み込み
	virtual void LoadStage(Stage name = Stage::Stage1) override;
	//ステージ追加
	virtual void AddStage() override;

	//ステージサイズ取得
	virtual Vector2 GetStageSize() const override {
		return stageSize_;
	}
	virtual void CreateClothes();
protected:
	//ピン追加関数
	void Pin_Add(int i, int j, int data, int laneNum);
	//服追加関数
	void Clothes_Add(int i, int j, int data, int laneNum);
	//ゴール追加関数（２種類のゴール）
	void GoalClothes_Add(int i, int j, int data, int laneNum);

protected:
	//ピンがつくかどうか
	std::queue<bool> pin_list;
	//服それぞれのスプライトのマップ
	std::map<CLOTHES_ID, std::vector<SPRITE_ID>> spriteIdMap_;
	//服が揺れる確率
	int frequencyWind_;
	//毛玉が飛んでくる確率
	int frequencyHairball_;
	//毛玉の飛ぶ頻度
	int hairballCnt_;
	Stage currentStage_;
};