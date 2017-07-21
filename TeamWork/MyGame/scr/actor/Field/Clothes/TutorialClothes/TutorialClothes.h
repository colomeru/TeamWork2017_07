#pragma once
#include "../Clothes.h"

class TutorialClothes : public Clothes, public std::enable_shared_from_this<TutorialClothes>
{
public:
	//コンストラクタ
	TutorialClothes(
		IWorld* world,
		int laneNum, 
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		const CLPoints& localPoints,
		bool is_Pin = false);
	//デストラクタ
	virtual ~TutorialClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;

private:
	//当たり判定の範囲を描画
	void DrawRange() const;
};