#include "Clothes.h"
#include "../MyGame/scr/math/MyFuncionList.h"
#include "../MyGame/scr/game/Random.h"
#include "Hanger\Hanger.h"
#include "../MyGame/scr/scene/GamePlayDefine.h"
#include "../MyGame/scr/tween/TweenManager.h"
#include "ClothesFeces\ClothesFeces.h"
#include "DropClothes\DropClothes.h"
#include "../../Effects/PlayerEffect/SwordAttackEffect.h"
#include "../../../sound/sound.h"
#include "../../player/Player_Head.h"

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight)
	:Actor(world)
	, isHit_(false), isPendulum_(false), isFriction_(false), isWind_(false)
	, fulcrum_(0, 0), rot_spd_(0.08f), length_(125.0f), gravity_(0.3f), friction_(1.0f), count_(0)
	, clothesState_(ClothesState::WINDLESS), cuttingState_(CuttingState::Normal), weight_(weight), drawFrame_(0)
	, is_Droping_(false), clothesFeces_(nullptr), penState_(NONE), player_(nullptr), isDrawRange_(false), currentStage_(Stage::Stage1)
{
	clothes_ID = clothes;
	rot_ = Random::GetInstance().Range(88.0f, 92.0f);
	dNumber_ = 0.0f;

	localPoints.clear();
	collisionPoints.clear();
	localPoints.reserve(4);
	collisionPoints.reserve(4);

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [=](Actor& other)
	{
		player_ = static_cast<Player*>(&other);
	});
}

Clothes::~Clothes()
{
	Sound::GetInstance().StopSE(SE_ID::SLASH_SE);
	localPoints.clear();
	collisionPoints.clear();
	player_ = nullptr;
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
		if (cuttingState_ != CuttingState::Normal || clothes_ID == CLOTHES_ID::NOT_SLASH_CLOTHES) break;
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
		if (is_Droping_ || isPendulum_ || cuttingState_ != CuttingState::Normal) return;
		Vector2 pos = other.GetPosition() - fulcrum_;
		clothesFeces_ = std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor());
		//world_->Add(ACTOR_ID::CLOTHES_DROPING_ACTOR, std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor()));
		is_Droping_ = true;
		other.Dead();
		if (player_ == nullptr || parent_ == nullptr) return;
		player_->SetMode(MODE_SLIP);
		parent_ = nullptr;
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
		if (rand > frequencyWind_) break;
		float dRand = Random::GetInstance().Range(0.0f, 1.5f);
		TweenManager::GetInstance().Delay(
			dRand,
			[&]() {
			rot_spd_ = 1.7f;
			rot_spd_ -= weight_;
			clothesState_ = ClothesState::BEGIN_WIND; },
			&dNumber_
			);
		break;
	}
	}
}

void Clothes::Pendulum(Vector2 fulcrum, float length)
{
	beforePos_ = position_;

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

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//角度を画像に反映
	angle_ = rot_ - 90;
	
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

void Clothes::Pendulum(std::vector<Vector2> & deformPos, float rot_spd, float length)
{
	for (int i = 2; i < deformPos.size(); i++) {
		PenStateDecision();
		auto dRot = MathHelper::Abs(rot_spd);
		auto fulcrum = deformPos[i - 2];
		auto rot = MathHelper::ATan(deformPos[i].y - deformPos[i - 2].y, deformPos[i].x - deformPos[i - 2].x);
		
		if (i < 4) {
			length = 50;
		}
		else if (i < 6) {
			length = 190;
		}
		
		//現在の重りの位置
		deformPos[i].x = fulcrum.x + MathHelper::Cos(rot) * length;
		deformPos[i].y = fulcrum.y + MathHelper::Sin(rot) * length;

		//重力移動量を反映した重りの位置
		auto length_vec = deformPos[i] - fulcrum;
		auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
		auto gx = deformPos[i].x + t * length_vec.x;
		auto gy = deformPos[i].y + gravity_ + t * length_vec.y;

		//2つの重りの位置の角度差
		auto r = MathHelper::ATan(gy - fulcrum.y, gx - fulcrum.x);

		//角度差を角速度に加算
		auto sub = r - rot;
		sub -= std::floor(sub / 360.0f) * 360.0f;
		if (sub < -180.0f) sub += 360.0f;
		if (sub > 180.0f) sub -= 360.0f;

		auto temp = rot_spd + sub;

		rot_spd = temp;
		//rot_spd_ += sub;

		//摩擦
		rot *= friction_;

		//角度に角速度を加算
		rot += rot_spd;

		//新しい重りの位置
		deformPos[i].x = fulcrum.x + MathHelper::Cos(rot) * length;
		deformPos[i].y = fulcrum.y + MathHelper::Sin(rot) * length;

		//switch (penState_)
		//{
		//case Clothes::CENTER_LEFT: {
		//	if (i == 2)
		//		deformPos[i].y *= 1.01f;
		//	else if (i == 3)
		//		deformPos[i].y *= 0.99f;
		//	break;
		//}
		//case Clothes::LEFT_CENTER: {
		//	if (i == 2)
		//		deformPos[i].y *= 0.99f;
		//	else if (i == 3)
		//		deformPos[i].y *= 1.01f;
		//	break;
		//}
		//case Clothes::CENTER_RIGHT: {
		//	if (i == 2)
		//		deformPos[i].y *= 0.99f;
		//	else if (i == 3)
		//		deformPos[i].y *= 1.01f;
		//	break;
		//}
		//case Clothes::RIGHT_CENTER: {
		//	if (i == 2)
		//		deformPos[i].y *= 1.01f;
		//	else if (i == 3)
		//		deformPos[i].y *= 0.99f;
		//	break;
		//}
		//default:
		//	break;
		//}

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
	case CuttingState::Normal: {
		SetNormal();
		break;
	}
	case CuttingState::RightUpSlant: {
		clothesFeces_ = nullptr;
		is_Droping_ = false;
		SetRightUpSlant();
		break;
	}
	case CuttingState::LeftUpSlant: {
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

void Clothes::DrawRangeUpdate()
{
	if (player_->GetIsShootMode())
		isDrawRange_ = true;

	if (player_->GetIsBiteMode())
		isDrawRange_ = false;
}

void Clothes::Synchronize()
{
	if (parent_ == nullptr || player_== nullptr || !player_->GetIsBiteMode()) return;

	pendulumVec_ = (position_ - beforePos_);
	auto pos = parent_->GetPosition() + pendulumVec_;
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));
	player_->setCurPHeadSPos(pos);
}

void Clothes::SetFrequencyWind(int wind)
{
	frequencyWind_ = wind;
}

void Clothes::SetCurrentStage(Stage currentStage)
{
	currentStage_ = currentStage;
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
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 90 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-50, -90 + length_, 0));
		localPoints.push_back(Vector3(-50, 95 + length_, 0));
		localPoints.push_back(Vector3(35, 95 + length_, 0));
		localPoints.push_back(Vector3(35, -90 + length_, 0));
		break;
	}
	case CLOTHES_ID::START_CLOTHES: {
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
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-50, -90 + length_, 0));
		localPoints.push_back(Vector3(-50, 55 + length_, 0));
		localPoints.push_back(Vector3(35, 30 + length_, 0));
		localPoints.push_back(Vector3(35, -90 + length_, 0));
		break;
	}
	case CLOTHES_ID::START_CLOTHES: {
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
	case CLOTHES_ID::GUM_CLOTHES: {
		localPoints.push_back(Vector3(-60, 0 + length_, 0));
		localPoints.push_back(Vector3(-60, 30 + length_, 0));
		localPoints.push_back(Vector3(60, 50 + length_, 0));
		localPoints.push_back(Vector3(60, 0 + length_, 0));
		break;
	}
	case CLOTHES_ID::THIN_CLOTHES: {
		localPoints.push_back(Vector3(-50, -90 + length_, 0));
		localPoints.push_back(Vector3(-50, 30 + length_, 0));
		localPoints.push_back(Vector3(35, 50 + length_, 0));
		localPoints.push_back(Vector3(35, -90 + length_, 0));
		break;
	}
	case CLOTHES_ID::START_CLOTHES: {
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

void Clothes::PenStateDecision()
{
	//中心から左
	if (rot_ > 90.0f && rot_spd_ > 0.0f)
		penState_ = PendulumState::CENTER_LEFT;
	//左から中心
	else if (rot_ > 90.0f && rot_spd_ < 0.0f)
		penState_ = PendulumState::LEFT_CENTER;
	//中心から右
	else if (rot_ < 90.0f && rot_spd_ < 0.0f)
		penState_ = PendulumState::CENTER_RIGHT;
	//右から中心
	else if (rot_ < 90.0f && rot_spd_ > 0.0f)
		penState_ = PendulumState::RIGHT_CENTER;

}

void Clothes::DrawRange() const
{
	if (currentStage_ != Stage::Stage1 || 
		collisionPoints.empty() ||
		!isDrawRange_) return;
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
}

