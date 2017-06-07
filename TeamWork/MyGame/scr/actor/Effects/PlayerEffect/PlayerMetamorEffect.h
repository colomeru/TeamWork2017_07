#pragma once
#include"../Effect.h"

class PlayerMetamorEffect:public Effect{
public:
	PlayerMetamorEffect(IWorld* world, const Vector2& position);

	void Update()override;
	void Draw()const override;

};
