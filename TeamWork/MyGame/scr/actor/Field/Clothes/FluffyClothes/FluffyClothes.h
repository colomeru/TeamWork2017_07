#pragma once
#include "../Clothes.h"

//ふわふわな服
class FluffyClothes : public Clothes, public std::enable_shared_from_this<FluffyClothes>
{
public:
	//コンストラクタ
	FluffyClothes(
		IWorld* world,
		int laneNum, 
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids, 
		const CLPoints& localPoints,
		bool is_Pin = false);
	//デストラクタ
	virtual ~FluffyClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;

};