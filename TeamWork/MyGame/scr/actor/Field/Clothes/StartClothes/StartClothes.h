#pragma once
#include "../Clothes.h"

class StartClothes : public Clothes, public std::enable_shared_from_this<StartClothes>
{
public:
	//コンストラクタ
	StartClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//デストラクタ
	virtual ~StartClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;

private:
	Actor* pActor;
	bool isStart_;
};