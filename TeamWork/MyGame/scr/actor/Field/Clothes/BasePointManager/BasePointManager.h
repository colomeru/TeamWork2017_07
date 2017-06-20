#pragma once
#include "../Clothes.h"
#include <map>
#include <vector>

class BasePointManager
{
public:
	//コンストラクタ
	BasePointManager();

	//
	void SetLocalPoints(SPRITE_ID spriteId, std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);

	//当たり判定ポイントの設定
	void SetBase01(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase02(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase03(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase04(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase05(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase06(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase07(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase08(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase09(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase10(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase11(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase12(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase13(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase14(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase15(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase16(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase17(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase18(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
	void SetBase19(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length);
};