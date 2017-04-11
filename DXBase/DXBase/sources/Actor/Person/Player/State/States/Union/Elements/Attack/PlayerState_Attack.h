#pragma once

#include "../../PlayerState_Union.h"

#include "../../../../../PlayerPtr.h"

#include "../../../../../PlayerBodyCollider.h"

class PlayerState_Attack : public PlayerState_Union {
public:
	PlayerState_Attack(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty);
private:
	virtual void unique_init() override;
	virtual void update(float deltaTime) override;
	virtual void collide(const Actor & other) override;
	virtual void end() override;
	virtual void input(float deltaTime) override;
private:
	Vector2 dir_;
	Vector2 launch_dir_;
	float launch_power_;
	float launch_dir_easeing_;
	Vector2 chase_dir_;
	float chase_power_;
	float chase_dir_easeing_;

	Vector2 gravity_;
	float gra_easeing_;

	PlayerBodyCollPtr attackColl_;
private:
	// プレイヤーポインタ
	PlayerBodyPtr launchBody_;
	// プレイヤーポインタ
	PlayerBodyPtr chaseBody_;

};

