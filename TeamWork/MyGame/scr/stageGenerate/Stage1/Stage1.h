#pragma once

#include "../IStageGenerate.h"

class Stage1 : public IStageGenerate
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
};