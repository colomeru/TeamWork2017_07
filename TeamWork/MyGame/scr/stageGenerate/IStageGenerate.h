#pragma once
#include "../world/IWorld.h"
#include "../input/CsvReader.h"

class IStageGenerate
{
public:
	//コンストラクタ
	IStageGenerate(IWorld* world, const std::string& fileName);
	//仮想デストラクタ
	virtual ~IStageGenerate();
	//ステージ読み込み
	virtual void LoadStage() = 0;
	//ステージ追加
	virtual void AddStage() = 0;

private:
	//コピー禁止
	IStageGenerate(const IStageGenerate& other) = default;
	IStageGenerate& operator = (const IStageGenerate& other) = default;

protected:
	//ワールド
	IWorld* world_;
	//リーダー
	CsvReader csvReader_;
	//
	std::string fileName_;

};

