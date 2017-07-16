#pragma once
#include "Clothes.h"
#include "BasePointManager\BasePointManager.h"

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//コンストラクタ
	BaseClothes(
		IWorld* world, 
		int laneNum, 
		Vector2 pos, 
		float weight, 
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		const std::map<CuttingState, std::vector<Vector3>>& localPoints,
		bool is_Pin = false
		);

	//デストラクタ
	virtual ~BaseClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
};