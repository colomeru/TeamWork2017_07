#include "ClothesFeces.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../Clothes.h"

ClothesFeces::ClothesFeces(IWorld * world, int laneNum, Vector2 pos, Actor* clothes)
	:Actor(world, clothes)
{
	parameter_.ID = ACTOR_ID::CLOTHES_DROPING_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(32.0f, 32.0f);

	laneNum_ = laneNum;
	position_ = pos;

	pos_ = pos;
	fulcrum_ = static_cast<Clothes*>(parent_)->GetFulcrum();

	//マトリクス情報
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(parent_->GetAngle())
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	SetPose(mat);
}

ClothesFeces::~ClothesFeces()
{
}

void ClothesFeces::Update()
{
	angle_ = parent_->GetAngle();
	//マトリクス情報
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	SetPose(mat);
}

void ClothesFeces::Draw() const
{
	auto pos = GetDrawPosVect(position_) + parent_->GetDrawAddPos();
	Vector2 size = Sprite::GetInstance().GetSize(SPRITE_ID::BIRDS_DROPPING_SPRITE);
	Vector2 origin = Vector2(size.x / 3, size.y);
	Sprite::GetInstance().Draw(
		SPRITE_ID::BIRDS_DROPPING_SPRITE, pos, origin, parent_->GetParameter().spriteAlpha_, Vector2(3.0f, 1.0f), angle_);
}
