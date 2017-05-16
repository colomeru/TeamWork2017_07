#include "StartUI.h"
#include "../../Def.h"

StartUI::StartUI(IWorld * world, Vector2 pos)
	:Actor(world), timeCount_(120)
{
	position_ = pos;
}

StartUI::~StartUI()
{
}

void StartUI::Update()
{
	timeCount_--;

	if (timeCount_ <= 0) {
		parameter_.isDead = true;
	}
}

void StartUI::Draw() const
{
	if (timeCount_ != 0){
		float spritealpha = timeCount_ / 120.f;
		spritealpha *= 5.f;
		spritealpha = MathHelper::Clamp(spritealpha, 0.0f, 1.0f);

		Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::GAME_START_TEXT_SPRITE) / 2;
		Vector2 txtSize = Vector2::One;
		if (timeCount_ >= 120)txtSize *= 1.5f;
		Sprite::GetInstance().Draw(SPRITE_ID::GAME_START_TEXT_SPRITE, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), crcOrigin, spritealpha, txtSize);
	}
}

void StartUI::OnUpdate()
{
}

void StartUI::OnCollide(Actor & other, CollisionParameter colpara)
{

}

void StartUI::OnMessage(EventMessage message, void * param)
{
}
