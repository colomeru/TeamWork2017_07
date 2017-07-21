#include "GoalUI.h"
#include "../../Def.h"
#include "../../tween/TweenManager.h"
#include "../../graphic/Sprite.h"

GoalUI::GoalUI(IWorld * world, Vector2 pos)
	:Actor(world), timeCount_(120), txtSize_(Vector2(0.7f, 0.7f))
{
	position_ = pos;
	TweenManager::GetInstance().Add(EaseOutElastic, &txtSize_, Vector2::One, 0.2f);
}

GoalUI::~GoalUI()
{
}

void GoalUI::Update()
{
	timeCount_--;

	if (timeCount_ == 0){
		world_->sendMessage(EventMessage::GAME_CLEAR_FLAG);
		//world_->SetIsGameClear(true);
		parameter_.isDead = true;
	}
}

void GoalUI::Draw() const
{
	if (timeCount_ != 0) {
		float spritealpha = timeCount_ / 120.f;
		spritealpha *= 5.f;
		spritealpha = MathHelper::Clamp(spritealpha, 0.0f, 1.0f);
		
		Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::GOAL_UI_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::GOAL_UI_SPRITE, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), crcOrigin, spritealpha, txtSize_);
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
