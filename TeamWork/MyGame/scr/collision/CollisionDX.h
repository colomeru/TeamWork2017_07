#pragma once
#include "Collision.h"
#include <vector>

enum ModelCollType
{
	All,
	Wall,
	Floor
};

// モデル
struct ModelData
{
	int		modelHandle;
	int		modelFrameIndex = -1;

	ModelData(int handle = 0, int frameIndex = -1) :
		modelHandle(handle), modelFrameIndex(frameIndex)
	{}
};

struct CollisionDX
{
	// カプセルとカプセルの当たり判定
	static bool Capsule_vs_Capsule(const Capsule& c1, const Capsule& c2);
	// カプセルとカプセルの当たり判定
	static CollisionParameter Capsule_vs_Capsule(CollisionParameter& colpara, const Capsule& c1, const Capsule& c2);
	// カプセルとポリゴンの当たり判定
	static bool Capsule_vs_Triangle(const Capsule& capsule, const Triangle& poly);
	// カプセルとポリゴンの当たり判定
	static CollisionParameter Capsule_vs_Triangle(CollisionParameter& colpara, const Capsule& capsule, const Triangle& poly);

	// モデルと線分の衝突判定
	static CollisionParameter Model_vs_Line(CollisionParameter& colpara, const ModelData& model, const Line& line);
	// モデルと球の衝突判定
	static CollisionParameter Model_vs_Sphere(CollisionParameter& colpara, const ModelData& model, const Sphere& sphere);
	// モデルとカプセルの衝突判定
	static CollisionParameter Model_vs_Capsule(CollisionParameter& colpara, std::vector<MV1_COLL_RESULT_POLY>& colpoly, const ModelData& model, const Capsule& capsule, ModelCollType type = ModelCollType::All);
};