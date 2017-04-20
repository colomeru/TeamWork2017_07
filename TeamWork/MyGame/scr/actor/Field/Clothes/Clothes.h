#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"

class Clothes : public Actor
{
public:
	//コンストラクタ
	Clothes(IWorld* world, CLOTHES_ID clothes);
	//仮想デストラクタ
	virtual ~Clothes() = default;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() const = 0;

	//IDの取得
	CLOTHES_ID GetClothesID() const {
		return clothes_ID;
	}

protected:
	//衝突しているか
	bool isHit_;
	//服ID
	CLOTHES_ID clothes_ID;
};