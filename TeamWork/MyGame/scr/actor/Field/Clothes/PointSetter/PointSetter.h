#pragma once
#include "../../../../input/CsvReader.h"
#include "../Clothes.h"

class PointSetter
{
public:
	//コンストラクタ
	PointSetter();
	//デストラクタ
	~PointSetter();
	//csv読み込み
	void Load(const std::string& fileName);
	//ポイント設定
	void SetLocalPoints(const SPRITE_ID& startSpriteId, float length);
	//ポイント取得
	std::map<CuttingState, std::vector<Vector3>> GetLocalPoints(const SPRITE_ID& spriteId) const;

private:
	//csvリーダー
	CsvReader csvReader_;
	//服の当たり判定ポイントマップ
	std::map<SPRITE_ID, std::map<CuttingState, std::vector<Vector3>>> spritePoints_;

};