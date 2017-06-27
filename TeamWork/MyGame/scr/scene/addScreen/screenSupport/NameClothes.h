#pragma once
#include"../../../actor/Actor.h"


class NameClothes :public Actor {
public:
	NameClothes(IWorld* world, const Vector2& position,const Vector2& targetPos,SPRITE_ID spriteId,int rectFrame);
	virtual void Update()override;
	virtual void Draw()const override;
private:
	SPRITE_ID spriteId_;
	int rectFrame_;
};