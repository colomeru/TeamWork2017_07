#include "NotShakeClothes.h"

NotShakeClothes::NotShakeClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::BASE_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);
	laneNum_ = laneNum;

	position_ = pos - Vector2(0, 20);
	fulcrum_ = position_ - Vector2(0, length_);
	spriteId_ = spriteId;

	SetLocalPoints();
	SetPointsUpdate();
}

NotShakeClothes::~NotShakeClothes()
{
}

void NotShakeClothes::Update()
{
	if (parent_ != nullptr) {
		if (!static_cast<Player*>(parent_->GetParent())->GetIsBiteMode()) {
			parent_ = nullptr;
		}
	}

	SetPointsUpdate();
	UpdateClothesFeces();
	DrawRangeUpdate();
}

void NotShakeClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(spriteId_) / 2;
	Sprite::GetInstance().SplitDraw(spriteId_, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
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

void NotShakeClothes::SetLocalPoints()
{
	switch (spriteId_)
	{
	case SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE: {
		SetPoints01();
		break;
	}
	case SPRITE_ID::NOT_SHAKE_CLOTHES_02_SPRITE: {
		SetPoints02();
		break;
	}
	default:
		break;
	}
}

void NotShakeClothes::SetPoints01()
{
	localPoints_[CuttingState::Normal].push_back(Vector3(-65, 90 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(65, 90 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-65, 10 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(65, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-65, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(65, 15 + length_, 0));
}

void NotShakeClothes::SetPoints02()
{
	localPoints_[CuttingState::Normal].push_back(Vector3(-65, 90 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(65, 90 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-65, 15 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(65, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-65, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-65, -70 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(65, -70 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(65, 20 + length_, 0));
}
