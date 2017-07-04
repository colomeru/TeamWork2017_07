#include "TutorialClothes.h"

TutorialClothes::TutorialClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::FLUFFY_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);
	laneNum_ = laneNum;

	position_ = pos + Vector2(0, -10);
	fulcrum_ = position_ - Vector2(0, length_);
	spriteId_ = spriteId;

	SetLocalPoints();

	SetPointsUpdate();
}

TutorialClothes::~TutorialClothes()
{
	baseLocalPoints_.clear();
}

void TutorialClothes::Update()
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
}

void TutorialClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(spriteId_) / 2;
	Sprite::GetInstance().SplitDraw(spriteId_, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	DrawClothesFeces();

	if (!collisionPoints.empty()) {
		DrawRange();
	}
}

void TutorialClothes::OnUpdate()
{
}

void TutorialClothes::SetLocalPoints()
{
	pointManager_.SetLocalPoints(spriteId_, baseLocalPoints_, length_);
}

void TutorialClothes::DrawRange() const
{
	auto topSize = Sprite::GetInstance().GetSize(SPRITE_ID::BITE_RANGE_TOP_SPRITE);
	auto origin = Sprite::GetInstance().GetSize(SPRITE_ID::BITE_RANGE_SPRITE) / 2;
	auto topOrigin = topSize / 2;
	Vector2 drawP[4];
	for (int i = 0; i < 4; i++) {
		drawP[i] = GetDrawPosVect(collisionPoints[i]);
	}
	Vector2 modiPos[4][4];
	for (int i = 0; i < 4; i++) {
		//collisionPointから見て、左、右、上、下
		modiPos[i][0] = drawP[i] + Vector2(-topSize.x / 2, 0);
		modiPos[i][1] = drawP[i] + Vector2(topSize.x / 2, 0);
		modiPos[i][2] = drawP[i] + Vector2(0, -topSize.x / 2);
		modiPos[i][3] = drawP[i] + Vector2(0, topSize.x / 2);
	}
	auto handle = Sprite::GetInstance().GetHandle(SPRITE_ID::BITE_RANGE_SPRITE);
	DrawModiGraph(modiPos[0][0].x, modiPos[0][0].y, modiPos[0][1].x, modiPos[0][1].y, 
		modiPos[1][1].x, modiPos[1][1].y, modiPos[1][0].x, modiPos[1][0].y, handle, true);
	DrawModiGraph(modiPos[1][2].x, modiPos[1][2].y, modiPos[2][2].x, modiPos[2][2].y, 
		modiPos[2][3].x, modiPos[2][3].y, modiPos[1][3].x, modiPos[1][3].y, handle, true);
	DrawModiGraph(modiPos[3][0].x, modiPos[3][0].y, modiPos[3][1].x, modiPos[3][1].y, 
		modiPos[2][1].x, modiPos[2][1].y, modiPos[2][0].x, modiPos[2][0].y, handle, true);

	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[0], topOrigin, Vector2::One, angle_);
	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[1], topOrigin, Vector2::One, angle_);
	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[2], topOrigin, Vector2::One, angle_);
	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[3], topOrigin, Vector2::One, angle_);

	if (BuildMode != 1) return;
	DrawCircle(drawP[0].x, drawP[0].y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP[1].x, drawP[1].y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP[2].x, drawP[2].y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP[3].x, drawP[3].y, parameter_.radius, GetColor(255, 255, 255));
	DrawLine(drawP[0].x, drawP[0].y, drawP[1].x, drawP[1].y, GetColor(255, 255, 255));
	DrawLine(drawP[1].x, drawP[1].y, drawP[2].x, drawP[2].y, GetColor(255, 255, 255));
	DrawLine(drawP[2].x, drawP[2].y, drawP[3].x, drawP[3].y, GetColor(255, 255, 255));
}

void TutorialClothes::SetPointsUpdate()
{
	collisionPoints.clear();

	if (laneNum_ != world_->GetKeepDatas().playerLane_ || !isDraw_) return;

	//ワールドマトリクス
	Matrix mat = Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	auto p1
		= Matrix::CreateTranslation(baseLocalPoints_[cuttingState_][0]) * mat;
	auto p2
		= Matrix::CreateTranslation(baseLocalPoints_[cuttingState_][1]) * mat;
	auto p3
		= Matrix::CreateTranslation(baseLocalPoints_[cuttingState_][2]) * mat;
	auto p4
		= Matrix::CreateTranslation(baseLocalPoints_[cuttingState_][3]) * mat;

	collisionPoints.push_back(Vector2(p1.Translation().x, p1.Translation().y));
	collisionPoints.push_back(Vector2(p2.Translation().x, p2.Translation().y));
	collisionPoints.push_back(Vector2(p3.Translation().x, p3.Translation().y));
	collisionPoints.push_back(Vector2(p4.Translation().x, p4.Translation().y));
}
