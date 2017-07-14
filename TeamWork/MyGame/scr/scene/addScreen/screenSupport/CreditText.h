#pragma once
#include "../../../actor/player/CreditPlayer.h"
#include "../../../actor/Field/Clothes/Clothes.h"

class CreditText : public Clothes/*,std::enable_shared_from_this<CreditText>*/ {
public:
	//コンストラクタ
	CreditText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame);
	//デストラクタ
	~CreditText();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//衝突判定
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	CreditPlayer* cPlayer_;
	int frame_;	//フレーム数
	float f2;	//前の値
};
