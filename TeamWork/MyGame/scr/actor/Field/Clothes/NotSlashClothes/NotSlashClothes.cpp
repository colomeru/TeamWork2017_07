#include "NotSlashClothes.h"

NotSlashClothes::NotSlashClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::NOT_SLASH_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);
	laneNum_ = laneNum;

	position_ = pos - Vector2(0, 45);
	fulcrum_ = position_ - Vector2(0, length_);
	spriteId_ = spriteId;

	SetLocalPoints();

	SetPointsUpdate();
}

NotSlashClothes::~NotSlashClothes()
{
	localPoints.clear();
}

void NotSlashClothes::Update()
{
	if (parent_ != nullptr) {
		if (!static_cast<Player*>(parent_->GetParent())->GetIsBiteMode()) {
			parent_ = nullptr;
		}
	}

	ShakesClothes();
	WindSwing();
	Synchronize();
	SetPointsUpdate();
	UpdateClothesFeces();
	DrawRangeUpdate();
}

void NotSlashClothes::Draw() const
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

void NotSlashClothes::OnUpdate()
{
}

void NotSlashClothes::SetPointsUpdate()
{
	collisionPoints.clear();

	if (laneNum_ != world_->GetKeepDatas().playerLane_ || !isDraw_) return;

	//ワールドマトリクス
	Matrix mat = Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	auto p1
		= Matrix::CreateTranslation(localPoints[0]) * mat;
	auto p2
		= Matrix::CreateTranslation(localPoints[1]) * mat;
	auto p3
		= Matrix::CreateTranslation(localPoints[2]) * mat;
	auto p4
		= Matrix::CreateTranslation(localPoints[3]) * mat;

	collisionPoints.push_back(Vector2(p1.Translation().x, p1.Translation().y));
	collisionPoints.push_back(Vector2(p2.Translation().x, p2.Translation().y));
	collisionPoints.push_back(Vector2(p3.Translation().x, p3.Translation().y));
	collisionPoints.push_back(Vector2(p4.Translation().x, p4.Translation().y));
}

void NotSlashClothes::SetLocalPoints()
{
	switch (spriteId_)
	{
	case SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE: {
		SetPoints01();
		break;
	}
	case SPRITE_ID::NOT_SLASH_CLOTHES_02_SPRITE: {
		SetPoints02();
		break;
	}
	default:
		break;
	}
}

void NotSlashClothes::SetPoints01()
{
	localPoints.push_back(Vector3(-65, 70 + length_, 0));
	localPoints.push_back(Vector3(-65, 20 + length_, 0));
	localPoints.push_back(Vector3(65, 20 + length_, 0));
	localPoints.push_back(Vector3(65, 70 + length_, 0));
}

void NotSlashClothes::SetPoints02()
{
	localPoints.push_back(Vector3(-70, 10 + length_, 0));
	localPoints.push_back(Vector3(-70, 55 + length_, 0));
	localPoints.push_back(Vector3(70, 55 + length_, 0));
	localPoints.push_back(Vector3(70, 10 + length_, 0));
}
