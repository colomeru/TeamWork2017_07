#ifndef ENEMY_CREATE_BOX_H_
#define ENEMY_CREATE_BOX_H_

#include "../../../Base/Actor.h"

class EnemyCreateBox : public Actor {
public:
	EnemyCreateBox(
		IWorld * world,
		const Vector2& position,
		float direction = 1.0f);
	EnemyCreateBox(
		IWorld * world,
		const Vector2& position,
		const Vector2& direction = Vector2(-1.0f, -1.0f));
	void onUpdate(float deltaTime) override;
	void onDraw() const override;
	void onCollide(Actor& actor) override;
	void onMessage(EventMessage event, void*) override;

private:
	bool isEnemyCreate_;
};

#endif