#pragma once
#include "../Clothes.h"
#include <map>

class NotShakeClothes : public Clothes, public std::enable_shared_from_this<NotShakeClothes>
{
public:
	//コンストラクタ
	NotShakeClothes(
		IWorld* world, 
		int laneNum, 
		Vector2 pos, 
		float weight, 
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		std::map<CuttingState, std::vector<Vector3>> localPoints,
		bool is_Pin = false);
	//デストラクタ
	virtual ~NotShakeClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
};