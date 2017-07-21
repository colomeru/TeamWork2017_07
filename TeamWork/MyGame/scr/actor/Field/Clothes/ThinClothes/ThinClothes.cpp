#include "ThinClothes.h"

ThinClothes::ThinClothes(
	IWorld * world,
	int laneNum,
	Vector2 pos, 
	float weight, 
	std::pair<CLOTHES_ID, SPRITE_ID> ids,
	std::map<CuttingState, std::vector<Vector3>> localPoints,
	bool is_Pin)
	:Clothes(world, ids.first, laneNum, weight, localPoints)
{
	clothes_ID = CLOTHES_ID::THIN_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 300.0f);

	spriteId_ = ids.second;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0.0f, LENGTH);

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

	if (!collisionPoints.empty()) {
		auto drawP1 = GetDrawPosVect(collisionPoints[0]);
		auto drawP2 = GetDrawPosVect(collisionPoints[1]);
		auto drawP3 = GetDrawPosVect(collisionPoints[2]);
		auto drawP4 = GetDrawPosVect(collisionPoints[3]);
		DebugDraw::DebugDrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
	}
}
