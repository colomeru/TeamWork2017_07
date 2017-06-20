#include "StartClothes.h"
#include "../../../player/Player_Head.h"
#include "../../../player//Player.h"
#include "../../../../Def.h"

StartClothes::StartClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::START_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);

	spriteId_ = SPRITE_ID::BASE_CLOTHES_SPRITE;
	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	SetLocalPoints();

	SetPointsUpdate();

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [&, this](const Actor& other) {
		pActor = static_cast<Player*>(const_cast<Actor*>(&other));
	});

	isStart_ = true;
}

StartClothes::~StartClothes()
{
	pActor = nullptr;
}

void StartClothes::Update()
{
	ShakesClothes();
	WindSwing();

	SetPointsUpdate();

	UpdateClothesFeces();

	auto player = static_cast<Player*>(const_cast<Actor*>(pActor));
	if (!player->GetIsBiteMode()) {
		isStart_ = false;
		return;
	}
	if (isStart_) {
		pendulumVec_ = (position_ - beforePos_);
		auto pos = player->GetCurrentPHeadPosition() + pendulumVec_;
		auto p = player->GetCurrentHead();
		player->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));
		player->setCurPHeadSPos(pos);
	}
	else Synchronize();
}

void StartClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, 20);
	Vector2 hangPos = GetDrawPosVect(fulcrum_);
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, hangPos, hangOrigin, parameter_.spriteAlpha_, Vector2(0.8f, 1.0f), angle_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::BASE_CLOTHES_SPRITE) / 2;
	Sprite::GetInstance().SplitDraw(SPRITE_ID::BASE_CLOTHES_SPRITE, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	DrawClothesFeces();

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
