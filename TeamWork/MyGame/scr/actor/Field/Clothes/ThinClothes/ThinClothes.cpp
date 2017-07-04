#include "ThinClothes.h"

ThinClothes::ThinClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::THIN_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100, 300.f);

	spriteId_ = SPRITE_ID::TOWEL_CLOTHES_SPRITE;
	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	SetLocalPoints();

	SetPointsUpdate();
}

ThinClothes::~ThinClothes()
{
}

void ThinClothes::Update()
{
	if (parent_ != nullptr) {
		if (!static_cast<Player*>(parent_->GetParent())->GetIsBiteMode()) {
			parent_ = nullptr;
		}
	}

	ShakesClothes();
	WindSwing();

	SetPointsUpdate();
	Synchronize();
	UpdateClothesFeces();

	DrawRangeUpdate();
}

void ThinClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::TOWEL_CLOTHES_SPRITE) / 2;
	Sprite::GetInstance().SplitDraw(SPRITE_ID::TOWEL_CLOTHES_SPRITE, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	DrawClothesFeces();
	DrawRange();

	if (BuildMode != 1) return;
	if (!collisionPoints.empty()) {
		auto drawP1 = GetDrawPosVect(collisionPoints[0]);
		auto drawP2 = GetDrawPosVect(collisionPoints[1]);
		auto drawP3 = GetDrawPosVect(collisionPoints[2]);
		auto drawP4 = GetDrawPosVect(collisionPoints[3]);
		DrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
		DrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
		DrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
		DrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
	}
}

void ThinClothes::SetLocalPoints()
{
	localPoints_[CuttingState::Normal].push_back(Vector3(-50, -90 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(-50, 95 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(35, 95 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(35, -90 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-50, -90 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-50, 55 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(35, 30 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(35, -90 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-50, -90 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-50, 30 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(35, 50 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(35, -90 + length_, 0));
}
