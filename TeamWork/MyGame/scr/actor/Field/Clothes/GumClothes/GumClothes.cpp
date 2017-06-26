#include "GumClothes.h"
#include "../MyGame/scr/game/Random.h"
#include "../../ClothesPin.h"
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
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	spriteId_ = SPRITE_ID::BASE_CLOTHES_SPRITE;
	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_ + 15);

	SetLocalPoints();

	SetPointsUpdate();

	spriteSize_ = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::BASE_CLOTHES_SPRITE);
	//(左上、右上、左中段、右中段、左下、右下)
	deformPos_.push_back(Vector2(fulcrum_.x - spriteSize_.x / 2, fulcrum_.y));
	deformPos_.push_back(Vector2(fulcrum_.x + spriteSize_.x / 2, fulcrum_.y));
	deformPos_.push_back(deformPos_[0] + Vector2(0, 50));
	deformPos_.push_back(deformPos_[1] + Vector2(0, 50));
	deformPos_.push_back(deformPos_[2] + Vector2(0, spriteSize_.y - 50));
	deformPos_.push_back(deformPos_[3] + Vector2(0, spriteSize_.y - 50));
	rot_ = 90.0f;
	rot_spd_ = -3.0f;

	for (int i = 0; i < deformPos_.size(); i++) {
		rot_spds_.push_back(rot_spd_);
	}
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

	Deform();

	SetPointsUpdate();
	Synchronize();
	UpdateClothesFeces();
}

void GumClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::GUM_SPRITE) / 2;
	//Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, 15);
	//Vector2 hangPos = GetDrawPosVect(fulcrum_);
	//Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, hangPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	//Sprite::GetInstance().SplitDraw(SPRITE_ID::GUM_SPRITE, drawPos, drawFrame_, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	//(左上、右上、左中段、右中段、左下、右下)
	auto dPos0 = GetDrawPosVect(deformPos_[0]);
	auto dPos1 = GetDrawPosVect(deformPos_[1]);
	auto dPos2 = GetDrawPosVect(deformPos_[2]);
	auto dPos3 = GetDrawPosVect(deformPos_[3]);
	auto dPos4 = GetDrawPosVect(deformPos_[4]);
	auto dPos5 = GetDrawPosVect(deformPos_[5]);
	
	//DrawRectModiGraph(dPos0.x, dPos0.y, dPos1.x, dPos1.y, dPos3.x, dPos3.y, dPos2.x, dPos2.y,
	//	0, 0, spriteSize_.x, 50, Sprite::GetInstance().GetHandle(SPRITE_ID::BASE_CLOTHES_SPRITE, drawFrame_), 1);
	//DrawRectModiGraph(dPos2.x, dPos2.y, dPos3.x, dPos3.y, dPos5.x, dPos5.y, dPos4.x, dPos4.y,
	//	0, 50, spriteSize_.x, spriteSize_.y - 50, Sprite::GetInstance().GetHandle(SPRITE_ID::BASE_CLOTHES_SPRITE, drawFrame_), 1);

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

	DrawCircle(dPos2.x, dPos2.y, 16, GetColor(255, 255, 0));
	DrawCircle(dPos3.x, dPos3.y, 16, GetColor(255, 255, 0));
	DrawCircle(dPos4.x, dPos4.y, 16, GetColor(255, 255, 0));
	DrawCircle(dPos5.x, dPos5.y, 16, GetColor(255, 255, 0));
	DrawLine(dPos2.x, dPos2.y, dPos3.x, dPos3.y, GetColor(255, 255, 0));
	DrawLine(dPos3.x, dPos3.y, dPos5.x, dPos5.y, GetColor(255, 255, 0));
	DrawLine(dPos5.x, dPos5.y, dPos4.x, dPos4.y, GetColor(255, 255, 0));
	DrawLine(dPos4.x, dPos4.y, dPos2.x, dPos2.y, GetColor(255, 255, 0));
}

void GumClothes::OnUpdate()
{
}

void GumClothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (other.GetParameter().ID)
	{
	case ACTOR_ID::PLAYER_HEAD_ACTOR:
	{
		if (!isWind_ && !is_Droping_) {
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

		isHit_ = true;
		break;
	}
	case ACTOR_ID::ENEMY_ACTOR:
	{
		if (is_Droping_ || isPendulum_) return;
		Vector2 pos = other.GetPosition() - fulcrum_;
		clothesFeces_ = std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor());
		//world_->Add(ACTOR_ID::CLOTHES_DROPING_ACTOR, std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor()));
		is_Droping_ = true;
		other.Dead();
		if (player_ == nullptr) return;
		player_->SetMode(MODE_SLIP);
		player_ = nullptr;
		parent_ = nullptr;
		break;
	}
	}
}

void GumClothes::Deform()
{

	//auto r = 0.0f;
	//for (int i = 2; i < deformPos_.size(); i++) {
	//	r = MathHelper::ATan(deformPos_[i].y - deformPos_[i - 2].y, deformPos_[i].x - deformPos_[i - 2].x);
	//}
	Pendulum(deformPos_, rot_spd_, 0);

	//float dRot = MathHelper::Abs(rot_spd_);
	////float dRot = (MathHelper::Abs(rot_) - 90.0f) / 45.0f;
	////float dRot2 = MathHelper::Abs(rot_spd_);
	////float dRot2 = (MathHelper::Abs(rot_) - 90.0f) / 30.0f;
	////中心から左に振るとき
	//if (rot_ > 90.0f && rot_spd_ > 0.0f) {
	//	deformPos_[2] += Vector2(0, -dRot);
	//	deformPos_[3] += Vector2(0, dRot);
	//	//deformPos_[4] += Vector2(dRot2, -dRot);
	//	//deformPos_[5] += Vector2(dRot2, dRot);

	//}	
	////右から中心に振るとき
	//if (rot_ < 90.0f && rot_spd_ > 0.0f) {
	//	deformPos_[2] += Vector2(0, -dRot);
	//	deformPos_[3] += Vector2(0, dRot);
	//	//deformPos_[4] += Vector2(dRot2, -dRot);
	//	//deformPos_[5] += Vector2(dRot2, dRot);

	//}
	////左から中心に振るとき
	//if (rot_ > 90.0f && rot_spd_ < 0.0f) {
	//	deformPos_[2] += Vector2(0, dRot);
	//	deformPos_[3] += Vector2(0, -dRot);
	//	//deformPos_[4] += Vector2(-dRot2, dRot);
	//	//deformPos_[5] += Vector2(-dRot2, -dRot);
	//}
	////中心から右に振るとき
	//if (rot_ < 90.0f && rot_spd_ < 0.0f) {
	//	deformPos_[2] += Vector2(0, dRot);
	//	deformPos_[3] += Vector2(0, -dRot);
	//	//deformPos_[4] += Vector2(-dRot2, dRot);
	//	//deformPos_[5] += Vector2(-dRot2, -dRot);
	//}

	//auto r = std::floor(rot_);
	//if (r == 90) {
	//	deformPos_[2] = deformPos_[1] + Vector2(0, 50);
	//	deformPos_[3] = deformPos_[0] + Vector2(0, 50);
	//	deformPos_[4] = deformPos_[0] + Vector2(0, spriteSize_.y);
	//	deformPos_[5] = deformPos_[1] + Vector2(0, spriteSize_.y);
	//}
}
