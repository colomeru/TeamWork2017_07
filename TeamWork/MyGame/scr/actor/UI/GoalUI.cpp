#include "GoalUI.h"

GoalUI::GoalUI(IWorld * world, Vector2 pos)
	:Actor(world)
{
	position_ = pos;
}

GoalUI::~GoalUI()
{
}

void GoalUI::Update()
{
	
}

void GoalUI::Draw() const
{
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_GOAL_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::TEST_GOAL_SPRITE, Vector2(drawPos_.x, 500), crcOrigin, 1.0f, Vector2::One);
}

void GoalUI::OnUpdate()
{
}

void GoalUI::OnCollide(Actor & other, CollisionParameter colpara)
{
	
}

void GoalUI::OnMessage(EventMessage message, void * param)
{
}
