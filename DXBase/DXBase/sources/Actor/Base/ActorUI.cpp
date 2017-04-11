#include "ActorUI.h"

ActorUI::ActorUI(IWorld * world, const Vector2 & position) :
	world_(world),
	position_(position),
	dead_(false),
	alpha_(1.0f){
}

void ActorUI::update(float deltaTime){
	onUpdate(deltaTime);
}

void ActorUI::draw() const{
	onDraw();
}

bool ActorUI::isDead() const{
	return dead_;
}

void ActorUI::dead(){
	dead_ = true;
}

void ActorUI::onUpdate(float deltaTime){
}

void ActorUI::onDraw() const{
}
