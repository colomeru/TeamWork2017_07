#pragma once
#include "Clothes.h"
#include "BasePointManager\BasePointManager.h"

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//コンストラクタ
	BaseClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//デストラクタ
	virtual ~BaseClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//服の当たり判定の設定
	virtual void SetPointsUpdate() override;
	//切断状態による当たり判定のポイントの設定
	virtual void SetLocalPoints() override;

private:
	std::map<CuttingState, std::vector<Vector3>> baseLocalPoints_;
	BasePointManager pointManager_;
};