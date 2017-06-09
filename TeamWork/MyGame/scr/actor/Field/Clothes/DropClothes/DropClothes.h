#pragma once
#include "../../../Actor.h"

//落下する服
class DropClothes : public Actor
{
public:
	// コンストラクタ
	DropClothes(IWorld* world, Vector2 pos, int laneNum, SPRITE_ID spriteID, int drawFrame);
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() const override;

private:
	int drawFrame_;
};

