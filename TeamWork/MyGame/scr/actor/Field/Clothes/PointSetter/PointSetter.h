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
	//
	std::map<CuttingState, std::vector<Vector3>> GetLocalPoints(const SPRITE_ID& spriteId) const;

private:
	//csvリーダー
	CsvReader csvReader_;
	//ファイル名
	std::string fileName_;
	//服のスプライトID
	SPRITE_ID spriteId_;
	//服の当たり判定ポイントマップ
	std::map<SPRITE_ID, std::map<CuttingState, std::vector<Vector3>>> spritePoints_;
	//服の原点から支点までの長さ
	float LENGTH;

};