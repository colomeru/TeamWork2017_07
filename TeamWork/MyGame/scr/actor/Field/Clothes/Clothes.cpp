#include "Clothes.h"
#include "../MyGame/scr/math/MyFuncionList.h"
#include "../MyGame/scr/game/Random.h"
#include "../../player/Player.h"
#include "Hanger\Hanger.h"
#include "../MyGame/scr/scene/GamePlayDefine.h"
#include "../MyGame/scr/tween/TweenManager.h"
#include "ClothesFeces\ClothesFeces.h"
#include "DropClothes\DropClothes.h"
#include "../../Effects/PlayerEffect/SwordAttackEffect.h"

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight)
	:Actor(world)
	,isHit_(false), isPendulum_(false), isFriction_(false), isWind_(false)
	,fulcrum_(0, 0), rot_spd_(0.08f), length_(125.0f), gravity_(0.3f), friction_(1.0f),count_(0)
	,clothesState_(ClothesState::WINDLESS),cuttingState_(ClothesCuttingState::Normal),weight_(weight),drawFrame_(0)
	,is_Droping_(false),clothesFeces_(nullptr)
{
	rot_ = Random::GetInstance().Range(88.0f, 92.0f);
	dNumber_ = 0.0f;

	localPoints.clear();
	collisionPoints.clear();
	localPoints.reserve(4);
	collisionPoints.reserve(4);
}

Clothes::~Clothes()
{
	localPoints.clear();
	collisionPoints.clear();
}

//当たり判定処理
void Clothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (other.GetParameter().ID)
	{
	case ACTOR_ID::PLAYER_HEAD_ACTOR:
	{
		if (!isWind_ && !is_Droping_) {
			parent_ = &other;
			static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
			static_cast<Player*>(parent_->GetParent())->CurHeadBite(other.GetPosition());
			static_cast<Player*>(parent_->GetParent())->SetIsBiteMode(true);
			static_cast<Player*>(parent_->GetParent())->SetOtherClothesID_(clothes_ID);
			break;
		}
		break;
	}
	case ACTOR_ID::PLAYER_SWORD_ACTOR:
	{
		if (cuttingState_ == ClothesCuttingState::Normal)
		{
			int rand = Random::GetInstance().Range(0, 2);
			switch (rand)
			{
			case 0: {
				cuttingState_ = ClothesCuttingState::RightUpSlant;
				drawFrame_ = 1;
				SetSpriteID();
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<SwordAttackEffect>(world_, colpara.colPos));
				world_->Add(
					ACTOR_ID::CLOTHES_DROPING_ACTOR, 
					std::make_shared<DropClothes>(world_, position_, laneNum_, spriteId_, drawFrame_)
					);
				SetLocalPoints();
				break;
			}
			case 1: {
				cuttingState_ = ClothesCuttingState::LeftUpSlant;
				drawFrame_ = 2;
				SetSpriteID();
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<SwordAttackEffect>(world_, colpara.colPos));
				world_->Add(
					ACTOR_ID::CLOTHES_DROPING_ACTOR, 
					std::make_shared<DropClothes>(world_, position_, laneNum_, spriteId_, drawFrame_)
					);
				SetLocalPoints();
				break;
			}
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
		break;
	}
	}

}

//メッセージ処理
void Clothes::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
	{
		if (!isUpdate_ || isPendulum_) break;
		int rand = Random::GetInstance().Range(0, 100);
		if (rand > frequencyWind) break;
		float dRand = Random::GetInstance().Range(0.0f, 1.5f);
		TweenManager::GetInstance().Delay(
			dRand,
			[&]() {
			/*isPendulum_ = true;*/
			rot_spd_ = 2.2f;
			rot_spd_ -= weight_;
			basePosition_ = position_;
			clothesState_ = ClothesState::BEGIN_WIND; },
			&dNumber_
			);
		break;
	}
	}
}

void Clothes::Pendulum(Vector2 fulcrum, float length)
{
	//現在の重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//重力移動量を反映した重りの位置
	auto length_vec = position_ - fulcrum;
	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto move_weightX = position_.x + t * length_vec.x;
	auto move_weightY = position_.y + gravity_ + t * length_vec.y;

	//2つの重りの位置の角度差
	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);

	//角度差を角速度に加算
	auto sub = r - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;

	auto temp = rot_spd_ + sub;
	if (sign(rot_spd_) != sign(temp) && isFriction_) {
		friction_ *= 0.992f;
	}
	rot_spd_ = temp;
	//rot_spd_ += sub;

	//摩擦
	rot_ *= friction_;

	//角速度の制限
	//rot_spd_ = MathHelper::Clamp(rot_spd_, -2.8f, 2.8f);

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//角度を画像に反映
	auto angle = rot_ - 90;
	angle_ = angle;
	
	if (clothesState_ == ClothesState::WINDLESS) return;

	count_++;

	switch (count_)
	{
	case 300:
		clothesState_ = ClothesState::BEGIN_STRONG_WIND;
		break;
	case 600:
		clothesState_ = ClothesState::ATTENUATE_WIND;
		break;
	case 900:
		clothesState_ = ClothesState::POSSIBLE_BITE;
		break;
	case 1200:
		clothesState_ = ClothesState::END_WIND;
		break;
	default:
		break;
	}
}

void Clothes::ShakesClothes()
{
	if (isDraw_) {
		Pendulum(fulcrum_, length_);

		switch (clothesState_)
		{
		case ClothesState::BEGIN_STRONG_WIND: {
			rot_spd_ = 4.0f;
			rot_spd_ -= weight_;
			isWind_ = true;
			TweenManager::GetInstance().Delay(15.0f, [=]() {isWind_ = false; });
			clothesState_ = ClothesState::STRONG_WIND;
			WindSwing();
			break;
		}
		case ClothesState::ATTENUATE_WIND: {
			isFriction_ = true;
			break;
		}
		case ClothesState::POSSIBLE_BITE: {
			isWind_ = false;
			break;
		}
		case ClothesState::END_WIND: {
			rot_spd_ = 0.08f;
			rot_ = 90.0f;
			friction_ = 1.0f;
			angle_ = 0;
			position_ = basePosition_;
			isFriction_ = false;
			count_ = 0;
			isPendulum_ = false;
			clothesState_ = ClothesState::WINDLESS;
			world_->sendMessage(EventMessage::END_WIND);
			break;
		}
		default:
			break;
		}
	}
}

void Clothes::WindSwing()
{
	if (parent_ == nullptr) return;

	if (clothesState_ == ClothesState::STRONG_WIND) {
		static_cast<Player*>(parent_->GetParent())->SetMode(MODE_RESIST);
		static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(true);
		parent_ = nullptr;
	}
}

void Clothes::SetPointsUpdate()
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

void Clothes::SetLocalPoints()
{
	localPoints.clear();
	switch (cuttingState_)
	{
	case ClothesCuttingState::Normal: {
		SetNormal();
		break;
	}
	case ClothesCuttingState::RightUpSlant: {
		clothesFeces_ = nullptr;
		is_Droping_ = false;
		SetRightUpSlant();
		break;
	}
	case ClothesCuttingState::LeftUpSlant: {
		clothesFeces_ = nullptr;
		is_Droping_ = false;
		SetLeftUpSlant();
		break;
	}
	}
}

void Clothes::UpdateClothesFeces()
{
	if (clothesFeces_ != nullptr && is_Droping_ && cuttingState_ == Normal)
		clothesFeces_->Update();
}

void Clothes::DrawClothesFeces() const
{
	if (clothesFeces_ != nullptr && is_Droping_ && cuttingState_ == Normal)
		clothesFeces_->Draw();
}

void Clothes::SetNormal()
{
	switch (clothes_ID)
	{
	case CLOTHES_ID::TEST_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::BASE_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::FLUFFY_CLOTHES: {
		localPoints.push_back(Vector3(-90, -20 + length_, 0));
		localPoints.push_back(Vector3(-90, 90 + length_, 0));
		localPoints.push_back(Vector3(90, 90 + length_, 0));
		localPoints.push_back(Vector3(90, -20 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	default:
		break;
	}
}

void Clothes::SetRightUpSlant()
{
	switch (clothes_ID)
	{
	case CLOTHES_ID::TEST_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::BASE_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::FLUFFY_CLOTHES: {
		localPoints.push_back(Vector3(-90, -20 + length_, 0));
		localPoints.push_back(Vector3(-90, 50 + length_, 0));
		localPoints.push_back(Vector3(90, 10 + length_, 0));
		localPoints.push_back(Vector3(90, -20 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	default:
		break;
	}
}

void Clothes::SetLeftUpSlant()
{
	switch (clothes_ID)
	{
	case CLOTHES_ID::TEST_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::BASE_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::FLUFFY_CLOTHES: {
		localPoints.push_back(Vector3(-90, -20 + length_, 0));
		localPoints.push_back(Vector3(-90, 10 + length_, 0));
		localPoints.push_back(Vector3(90, 40 + length_, 0));
		localPoints.push_back(Vector3(90, -20 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	default:
		break;
	}

}

void Clothes::SetSpriteID()
{
	switch (clothes_ID)
	{
	case GUM_CLOTHES: {
		spriteId_ = SPRITE_ID::GUM_SPRITE;
		break;
	}
	case FLUFFY_CLOTHES: {
		spriteId_ = SPRITE_ID::FLUFFY_SPRITE;
		break;
	}
	case THIN_CLOTHES: {
		spriteId_ = SPRITE_ID::TOWEL_CLOTHES_SPRITE;
		break;
	}
	default:
		break;
	}
}
