#pragma once
#include "../Clothes.h"
#include <map>
#include "../BasePointManager\BasePointManager.h"

class TutorialClothes : public Clothes, public std::enable_shared_from_this<TutorialClothes>
{
public:
	//コンストラクタ
	TutorialClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//デストラクタ
	virtual ~TutorialClothes() override;
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
	//当たり判定の範囲を描画
	void DrawRange() const;
private:
	std::map<CuttingState, std::vector<Vector3>> baseLocalPoints_;
	BasePointManager pointManager_;
};