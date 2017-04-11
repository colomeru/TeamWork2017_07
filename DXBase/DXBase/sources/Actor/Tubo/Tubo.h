#pragma once
#include "../Base/Actor.h"
#include "../../World/IWorld.h"
#include "../../Math/Vector3.h"
#include "../../Animation/Base/Animation2D.h"
// ÉvÉåÅ[ÉÑÅ[
class Tubo : public Actor {
public:
	Tubo(IWorld* world, const Vector2& position);
	~Tubo();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other) override;

private:
	bool mDownFlag;
	Vector2 mPosition;
	Vector2 mVelo;

	float timer_;
};