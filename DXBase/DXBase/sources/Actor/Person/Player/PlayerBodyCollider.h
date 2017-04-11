#pragma once

#include "../../Base/Actor.h"
#include "../../../World/IWorld.h"
#include "PlayerPtr.h"

#include "Player.h"

// ÉvÉåÅ[ÉÑÅ[
class PlayerBodyCollider : public Actor {
public:
	PlayerBodyCollider(IWorld* world, std::string& name);
	~PlayerBodyCollider();
	virtual void onUpdate(float deltaTime) override;
	virtual void onCollide(Actor& other) override;
	virtual void onDraw() const override;

	void reset_param();

	void reset_pos();
	void reset_velocity();
	Vector2& other_velocity();
	Vector2 other_position();
private:
	Vector2 other_velocity_;
	Vector2 other_position_;

};
