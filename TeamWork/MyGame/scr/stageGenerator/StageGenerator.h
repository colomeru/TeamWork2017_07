#pragma once
#include "../world/IWorld.h"
#include "../input/CsvReader.h"
#include "Stage.h"

class StageGenerator
{
public:
	//コンストラクタ
	StageGenerator(IWorld* world, const std::string& fileName);
	//仮想デストラクタ
	virtual ~StageGenerator() = default;
	//ステージ読み込み
	virtual void LoadStage(Stage name = Stage::Stage1) = 0;
	//ステージ追加
	virtual void AddStage() = 0;
	//ステージサイズ取得
	virtual Vector2 GetStageSize() const = 0;

private:
	//コピー禁止
	StageGenerator(const StageGenerator& other) = delete;
	StageGenerator& operator = (const StageGenerator& other) = delete;

protected:
	//ワールド
	IWorld* world_;
	//リーダー
	CsvReader csvReader_;
	//csvファイル名
	std::string fileName_;
	//ステージサイズ
	Vector2 stageSize_;
	//ステージの1マスのサイズ
	const int STAGE_TIP_SIZE;
};

