#pragma once
#include "../Clothes.h"

//薄い生地の服
class ThinClothes : public Clothes, public std::enable_shared_from_this<ThinClothes>
{
public:
	//コンストラクタ
	ThinClothes(
		IWorld* world,
		int laneNum, 
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		std::map<CuttingState, std::vector<Vector3>> localPoints,
		bool is_Pin = false);
	//デストラクタ
	virtual ~ThinClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
};