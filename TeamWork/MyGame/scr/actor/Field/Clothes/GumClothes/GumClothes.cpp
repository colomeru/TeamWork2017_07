#include "GumClothes.h"
#include "../MyGame/scr/game/Random.h"
#include "../../../player/Player_Head.h"
#include "../../../Effects/PlayerEffect/SwordAttackEffect.h"
#include "../DropClothes\DropClothes.h"
#include "../ClothesFeces\ClothesFeces.h"
#include "../../../../sound/sound.h"

GumClothes::GumClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin)
	:Clothes(world, clothes, laneNum, weight)
{
	clothes_ID = CLOTHES_ID::GUM_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(100.0f, 200.0f);

	spriteId_ = SPRITE_ID::BASE_CLOTHES_SPRITE;
	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_ + 15);

	SetLocalPoints();

	SetPointsUpdate();
}

GumClothes::~GumClothes()
{
}

void GumClothes::Update()
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
}

void GumClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::GUM_SPRITE) / 2;
	Sprite::GetInstance().SplitDraw(SPRITE_ID::GUM_SPRITE, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

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

void GumClothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (other.GetParameter().ID)
	{
	case ACTOR_ID::PLAYER_HEAD_ACTOR:
	{
		if (!isWind_ && !isDroping_) {
			parent_ = &other;
			static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
			player_ = static_cast<Player*>(parent_->GetParent());
			player_->CurHeadBite(other.GetPosition());
			player_->SetIsBiteMode(true);
			player_->SetOtherClothesID_(clothes_ID);
			break;
		}
		break;
	}
	case ACTOR_ID::PLAYER_SWORD_ACTOR:
	{
		if (cuttingState_ != CuttingState::Normal) break;
		int rand = Random::GetInstance().Range(0, 2);
		switch (rand)
		{
		case 0: {
			cuttingState_ = CuttingState::RightUpSlant;
			drawFrame_ = 1;
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<SwordAttackEffect>(world_, colpara.colPos));
			world_->Add(
				ACTOR_ID::CLOTHES_DROPING_ACTOR,
				std::make_shared<DropClothes>(world_, position_, laneNum_, spriteId_, drawFrame_)
				);
			Sound::GetInstance().PlaySE(SE_ID::SLASH_SE);
			SetLocalPoints();
			break;
		}
		case 1: {
			cuttingState_ = CuttingState::LeftUpSlant;
			drawFrame_ = 2;
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<SwordAttackEffect>(world_, colpara.colPos));
			world_->Add(
				ACTOR_ID::CLOTHES_DROPING_ACTOR,
				std::make_shared<DropClothes>(world_, position_, laneNum_, spriteId_, drawFrame_)
				);
			Sound::GetInstance().PlaySE(SE_ID::SLASH_SE);
			SetLocalPoints();
			break;
		}
		}
		break;
	}
	case ACTOR_ID::ENEMY_ACTOR:
	{
		if (isDroping_) return;
		Vector2 pos = other.GetPosition() - fulcrum_;
		clothesFeces_ = std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor());
		isDroping_ = true;
		other.Dead();
		if (player_ == nullptr) return;
		player_->SetMode(MODE_SLIP);
		player_ = nullptr;
		parent_ = nullptr;
		break;
	}
	}
}

void GumClothes::SetLocalPoints()
{
	localPoints_[CuttingState::Normal].push_back(Vector3(-60, 0 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(-60, 90 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(60, 90 + length_, 0));
	localPoints_[CuttingState::Normal].push_back(Vector3(60, 0 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length_, 0));
	localPoints_[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length_, 0));
	localPoints_[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length_, 0));
}
