#pragma once
#include"../../../math/Vector2.h"
#include"../../../math/Vector3.h"

#include"../../../world/IWorld.h"
#include"../../../actor/Actor.h"

class BackgroundCharacters:public Actor{
public:
	BackgroundCharacters(IWorld* world,const Vector2& position)
		:Actor(world) {
		position_=position;
	}
	virtual ~BackgroundCharacters() = default;

	virtual void Update() = 0;
	virtual void Draw()const = 0;
	virtual void Del() = 0;
	Vector2 GetDrawPosVect(const Vector2& pos)const {
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);
		return retPos;
	}

protected:
};