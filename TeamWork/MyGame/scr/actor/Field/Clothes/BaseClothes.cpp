#include "BaseClothes.h"
#include "../MyGame/scr/game/Random.h"
#include "../ClothesPin.h"
#include "../MyGame/scr/game/Random.h"
#include "../../../input/Keyboard.h"
#include "../../../conv/DXConverter.h"
#include "../../../graphic/Model.h"
#include "../../../graphic/Sprite.h"

BaseClothes::BaseClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::BASE_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(200, 200.f);
	//parameter_.mat
	//	= Matrix::CreateScale(Vector3::One)
	//	* Matrix::CreateRotationZ(0.0f)
	//	* Matrix::CreateTranslation(Vector3(0, 0, 0));
	//parameter_.ClothSegmentPoints_.push_back(Vector2(-100.f, 100.f));
	//parameter_.ClothSegmentPoints_.push_back(Vector2(	0.f, 100.f));
	//parameter_.ClothSegmentPoints_.push_back(Vector2( 100.f, 100.f));

	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	SetLocalPoints();

	SetPointsUpdate();

	GraphicPattern();

	//colFuncMap_[COL_ID::BOX_SEGMENT_COL] = std::bind(&CollisionFunction::IsHit_PSword_Clothes, colFunc_, std::placeholders::_1, std::placeholders::_2);

	//if (is_Pin)
	//	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<ClothesPin>(world_, laneNum_, Vector2(50, 50), this, fulcrum_));

	//colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

BaseClothes::~BaseClothes()
{
}

void BaseClothes::Update()
{
	if (parent_ != nullptr) {
		if (!static_cast<Player*>(parent_->GetParent())->GetIsBiteMode()) {
			parent_ = nullptr;
		}
	}

	ShakesClothes();
	WindSwing();

	SetPointsUpdate();

	isHit_ = false;
}

void BaseClothes::Draw() const
{
	//Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_SPRITE) / 2;
	//Vector2 crcOrigin2 = Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_02_SPRITE) / 2;
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, 12);
	Vector2 hangPos = GetDrawPosVect(fulcrum_);
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, hangPos, hangOrigin, spriteAlpha_, Vector2::One, angle_);
	//Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_SPRITE, drawPos_, crcOrigin, spriteAlpha_, Vector2::One, angle_);
	//Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_02_SPRITE, drawPos_, crcOrigin2, spriteAlpha_, Vector2::One, angle_);
	PatternDraw();

	if (BuildMode != 1) return;
	if (!collisionPoints.empty()) {
		auto drawP1 = GetDrawPosVect(collisionPoints[0]);
		auto drawP2 = GetDrawPosVect(collisionPoints[1]);
		auto drawP3 = GetDrawPosVect(collisionPoints[2]);
		auto drawP4 = GetDrawPosVect(collisionPoints[3]);
		//RECT rect = { 0, 0, Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_SPRITE).x, Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_SPRITE).y / 1.5f };
		//Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_SPRITE, drawPos_, rect, crcOrigin, 1.0f, Vector2::One, angle_, true, false);
		DrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
		DrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
		DrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
		DrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
		DrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
	}

	auto drawP = GetDrawPosVect(intersectPos_);
	if (isHit_) {
		DrawCircle(drawP.x, drawP.y, 32, GetColor(255, 0, 0));
	}
}

void BaseClothes::OnUpdate()
{
}

void BaseClothes::GraphicPattern()
{
	int rand = Random::GetInstance().Range(0, 2);
	switch (rand)
	{
	case 0: {
		pattern_ = ClothesPattern::Orange;
		break;
	}
	case 1: {
		pattern_ = ClothesPattern::SkyBlue;
		break;
	}
	}
}

void BaseClothes::PatternDraw() const
{
	switch (pattern_)
	{
	case ClothesPattern::Orange: {
		Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_SPRITE, drawPos_, crcOrigin, spriteAlpha_, Vector2::One, angle_);
		break;
	}
	case ClothesPattern::SkyBlue: {
		Vector2 crcOrigin2 = Sprite::GetInstance().GetSize(SPRITE_ID::BASE_CLOTHES_02_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_02_SPRITE, drawPos_, crcOrigin2, spriteAlpha_, Vector2::One, angle_);
		break;
	}
	}
}
