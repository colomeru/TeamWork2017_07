#include "GoalUI.h"
#include "../../Def.h"

GoalUI::GoalUI(IWorld * world, Vector2 pos)
	:Actor(world), timeCount_(120)
{
	position_ = pos;
}

GoalUI::~GoalUI()
{
}

void GoalUI::Update()
{
	timeCount_--;

	if (timeCount_ == 0){
		world_->SetIsGameClear(true);
	}
}

void GoalUI::Draw() const
{
	if (timeCount_ != 0) {
		Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_GOAL_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::TEST_GOAL_SPRITE, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), crcOrigin, 1.0f, Vector2::One);
	}
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
