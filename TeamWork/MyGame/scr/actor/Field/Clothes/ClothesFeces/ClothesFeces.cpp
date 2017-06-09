#include "ClothesFeces.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../Clothes.h"

ClothesFeces::ClothesFeces(IWorld * world, int laneNum, Vector2 pos, Actor* clothes)
	:Actor(world, clothes)
{
	parameter_.ID = ACTOR_ID::CLOTHES_DROPING_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(32.f, 32.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;

	pos_ = pos;
	fulcrum_ = static_cast<Clothes*>(parent_)->GetFulcrum();

	//�}�g���N�X���
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(parent_->GetAngle())
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	SetPose(mat);
}

void ClothesFeces::Update()
{
	angle_ = parent_->GetAngle();
	//�}�g���N�X���
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	SetPose(mat);
}

void ClothesFeces::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::BIRDS_DROPPING_SPRITE, drawPos_, origin, spriteAlpha_, Vector2::One, angle_);
}