#include "TutorialClothes.h"
#include "../../../../graphic/Sprite.h"
#include "../../../../debugdata/DebugDraw.h"

TutorialClothes::TutorialClothes(
	IWorld * world,
	int laneNum,
	Vector2 pos,
	float weight,
	std::pair<CLOTHES_ID, SPRITE_ID> ids,
	const CLPoints& localPoints,
	bool is_Pin)
	:Clothes(world, ids.first, laneNum, weight, localPoints)
{
	clothes_ID = CLOTHES_ID::FLUFFY_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);
	laneNum_ = laneNum;

	position_ = pos + Vector2(0.0f, -10.0f);
	fulcrum_ = position_ - Vector2(0.0f, LENGTH);
	spriteId_ = ids.second;

	SetPointsUpdate();
}

TutorialClothes::~TutorialClothes()
{
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

void TutorialClothes::DrawRange() const
{
	Sprite& instance = Sprite::GetInstance();
	auto topSize = instance.GetSize(SPRITE_ID::BITE_RANGE_TOP_SPRITE);
	auto origin = instance.GetSize(SPRITE_ID::BITE_RANGE_SPRITE) / 2;
	auto topOrigin = topSize / 2;
	Vector2 codePos_[4];
	for (int i = 0; i < 4; i++) {
		codePos_[i] = GetDrawPosVect(collisionPoints[i]);
	}
	Vector2 modiPos[4][4];
	for (int i = 0; i < 4; i++) {
		//collisionPointから見て、左、右、上、下
		modiPos[i][0] = codePos_[i] + Vector2(-topSize.x / 2.0f, 0.0f);
		modiPos[i][1] = codePos_[i] + Vector2(topSize.x / 2.0f, 0.0f);
		modiPos[i][2] = codePos_[i] + Vector2(0.0f, -topSize.x / 2.0f);
		modiPos[i][3] = codePos_[i] + Vector2(0.0f, topSize.x / 2.0f);
	}
	auto handle = instance.GetHandle(SPRITE_ID::BITE_RANGE_SPRITE);
	DrawModiGraph(modiPos[0][0].x, modiPos[0][0].y, modiPos[0][1].x, modiPos[0][1].y, 
		modiPos[1][1].x, modiPos[1][1].y, modiPos[1][0].x, modiPos[1][0].y, handle, true);
	DrawModiGraph(modiPos[1][2].x, modiPos[1][2].y, modiPos[2][2].x, modiPos[2][2].y, 
		modiPos[2][3].x, modiPos[2][3].y, modiPos[1][3].x, modiPos[1][3].y, handle, true);
	DrawModiGraph(modiPos[3][0].x, modiPos[3][0].y, modiPos[3][1].x, modiPos[3][1].y, 
		modiPos[2][1].x, modiPos[2][1].y, modiPos[2][0].x, modiPos[2][0].y, handle, true);

	instance.Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, codePos_[0], topOrigin, Vector2::One, angle_);
	instance.Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, codePos_[1], topOrigin, Vector2::One, angle_);
	instance.Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, codePos_[2], topOrigin, Vector2::One, angle_);
	instance.Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, codePos_[3], topOrigin, Vector2::One, angle_);

	DebugDraw::DebugDrawCircle(codePos_[0].x, codePos_[0].y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(codePos_[1].x, codePos_[1].y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(codePos_[2].x, codePos_[2].y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(codePos_[3].x, codePos_[3].y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(codePos_[0].x, codePos_[0].y, codePos_[1].x, codePos_[1].y, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(codePos_[1].x, codePos_[1].y, codePos_[2].x, codePos_[2].y, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(codePos_[2].x, codePos_[2].y, codePos_[3].x, codePos_[3].y, GetColor(255, 255, 255));
}
