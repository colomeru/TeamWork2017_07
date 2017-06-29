#pragma once
#include "../Clothes.h"

//薄い生地の服
class ThinClothes : public Clothes, public std::enable_shared_from_this<ThinClothes>
{
public:
	//コンストラクタ
	ThinClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//デストラクタ
	virtual ~ThinClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
};