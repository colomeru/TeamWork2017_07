#pragma once
#include "../../Clothes.h"
#include <map>
#include <vector>

class FluffyPointManager
{
public:
	//コンストラクタ
	FluffyPointManager();

	//
	void SetLocalPoints(SPRITE_ID spriteId, std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);

	//当たり判定ポイントの設定
	void SetFluffy01(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetFluffy02(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetFluffy03(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetFluffy04(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetFluffy05(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetFluffy06(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
};