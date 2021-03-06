#pragma once
#include "../Clothes.h"

//ゴムの服
class GumClothes : public Clothes, public std::enable_shared_from_this<GumClothes>
{
public:
	//コンストラクタ
	GumClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//デストラクタ
	virtual ~GumClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	void Deform();

private:
	std::vector<Vector2> deformPos_;
	std::vector<float> rot_spds_;
	Vector2 spriteSize_;
};