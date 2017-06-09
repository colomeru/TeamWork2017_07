#pragma once
#include"../Actor.h"
#include"../player/PlayerEffectManager.h"
#include"../../world/IWorld.h"

class Effect:public Actor {
public:
	Effect(IWorld* world,SPRITE_ID id,int anmSpeed=5);
	virtual void Update()override;
	virtual void Draw()const override;

protected:
	PlayerEffectManager effectMgr_;
};