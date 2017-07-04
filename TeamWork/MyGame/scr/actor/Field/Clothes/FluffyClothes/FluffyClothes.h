#pragma once
#include "../Clothes.h"
#include "FluffyPointManager\FluffyPointManager.h"

//ふわふわな服
class FluffyClothes : public Clothes, public std::enable_shared_from_this<FluffyClothes>
{
public:
	//コンストラクタ
	FluffyClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//デストラクタ
	virtual ~FluffyClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//切断状態による当たり判定のポイントの設定
	virtual void SetLocalPoints() override;
	//服の当たり判定の設定
	virtual void SetPointsUpdate() override;

private:
	//ふわふわ服のローカルな当たり判定の位置
	std::map<CuttingState, std::vector<Vector3>> fluffyLocalPoints_;
	//当たり判定マネージャー
	FluffyPointManager pointManager_;
};