#include "PlayerBodyCollider.h"

#include "../../../Field/Field.h"

#include "../../../Define.h"

PlayerBodyCollider::PlayerBodyCollider(IWorld * world, std::string& name):
	Actor(world, name + "Collider", Vector2::Zero, CollisionBase(Vector2(0, 0), PLAYER_COLLIDER_RADIUS)){
}

PlayerBodyCollider::~PlayerBodyCollider(){}

void PlayerBodyCollider::onUpdate(float deltaTime){
	opponent_ = HitOpponent::NONE;

	//other_velocity_ = Vector2::Zero;

	//if (world_->getField() == nullptr)return;
	//if (world_->getField()->hitCheck(position_) == FieldEnum::Field) {
	//	opponent_ = HitOpponent::FLOOR_HIT;
	//}
}

void PlayerBodyCollider::onCollide(Actor & other) {
	if (other.getName() == "MovelessFloor"  ||
		other.getName() == "MoveFloorUpDown" || other.getName() == "MoveFloorRightLeft" ||
		other.getName() == "TurnFloor" || other.getName() == "TranslessTurnFloor"||
		other.getName() == "MovelessFloorBreak"|| other.getName() == "Door"||other.getName()=="BlockParticle") {
		opponent_ = HitOpponent::FLOOR_HIT;
		other_velocity_ = other.getBody().GetColliderVelocity();
		other_position_ = other.getBody().GetBox().position_;
	}
	if (other.getName() == "SegmentCollider") {
		opponent_ = HitOpponent::FLOOR_HIT;
	}
	if (other.getName() == "DeadEnemy") {
		opponent_ = HitOpponent::ITEM;
	}
}

void PlayerBodyCollider::onDraw() const{
	//DrawCircle((position_ * inv_).x, (position_ * inv_).y, PLAYER_COLLIDER_RADIUS, GetColor(255, 255, 255), 0);
	
	//if(name_ == "PlayerHoldCollider")DrawCircle((position_ * inv_).x, (position_ * inv_).y, PLAYER_COLLIDER_RADIUS, GetColor(255, 255, 255));
}

void PlayerBodyCollider::reset_param(){
}

void PlayerBodyCollider::reset_pos(){
	other_position_ = Vector2::Zero;
}

Vector2& PlayerBodyCollider::other_velocity(){
	return other_velocity_;
}

void PlayerBodyCollider::reset_velocity() {
	other_velocity_ = Vector2::Zero;
}

Vector2 PlayerBodyCollider::other_position(){
	return other_position_;
}

