#include "Clothes.h"
#include "../../../game/Random.h"
#include "../../../tween/TweenManager.h"
#include "ClothesFeces\ClothesFeces.h"
#include "DropClothes\DropClothes.h"
#include "../../Effects/PlayerEffect/SwordAttackEffect.h"
#include "../../../sound/sound.h"
#include "../../player/Player_Head.h"
#include "../../../graphic/Sprite.h"
#include "../../../debugdata/DebugDraw.h"

//重力加速度
const float GRAVITY = 0.3f;

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight, const CLPoints& localPoints)
	:Actor(world)
	, clothes_ID(clothes), isFriction_(false), isWind_(false), localPoints_(localPoints)
	, fulcrum_(0.0f, 0.0f), rot_spd_(0.08f), friction_(1.0f), count_(0)
	, clothesState_(ClothesState::WINDLESS), cuttingState_(CuttingState::Normal), dNumber_(0.0f), weight_(weight), drawFrame_(0)
	, isDroping_(false), clothesFeces_(nullptr), player_(nullptr), frequencyWind_(0), isDrawRange_(false), currentStage_(Stage::Stage1)
{
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;

	laneNum_ = laneNum;
	rot_ = Random::GetInstance().Range(88.0f, 92.0f);

	collisionPoints.clear();

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [=](Actor& other) { player_ = static_cast<Player*>(&other); });
}

Clothes::~Clothes()
{
	Sound::GetInstance().StopSE(SE_ID::SLASH_SE);
	localPoints_[CuttingState::Normal].clear();
	localPoints_[CuttingState::RightUpSlant].clear();
	localPoints_[CuttingState::LeftUpSlant].clear();
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
		if (!isWind_ && !isDroping_) {
			parent_ = &other;
			static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
			player_ = static_cast<Player*>(parent_->GetParent());
			player_->CurHeadBite(other.GetPosition());
			player_->SetIsBiteMode(true);
			player_->SetOtherClothesID_(clothes_ID);
		}
		break;
	}
	case ACTOR_ID::PLAYER_SWORD_ACTOR:
	{
		if (cuttingState_ != CuttingState::Normal || clothes_ID == CLOTHES_ID::NOT_SLASH_CLOTHES) break;
		isDroping_ = false;
		clothesFeces_ = nullptr;
		Sound::GetInstance().PlaySE(SE_ID::SLASH_SE);
		int rand = Random::GetInstance().Range(1, 3);
		cuttingState_ = (CuttingState)rand;
		drawFrame_ = rand;
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<SwordAttackEffect>(world_, colpara.colPos));
		world_->Add(
			ACTOR_ID::CLOTHES_DROPING_ACTOR,
			std::make_shared<DropClothes>(world_, position_, laneNum_, spriteId_, drawFrame_, angle_)
			);
		break;
	}
	case ACTOR_ID::ENEMY_ACTOR: 
	{
		if (isDroping_ || cuttingState_ != CuttingState::Normal) return;
		Vector2 pos = other.GetPosition() - fulcrum_;
		clothesFeces_ = std::make_shared<ClothesFeces>(world_, laneNum_, pos, this->GetActor());
		isDroping_ = true;
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
	if (message == EventMessage::BEGIN_WIND) {
		if (!isUpdate_) return;
		int rand = Random::GetInstance().Range(0, 100);
		if (rand > frequencyWind_) return;
		float dRand = Random::GetInstance().Range(0.0f, 1.0f);
		TweenManager::GetInstance().Delay(
			dRand,
			[&]() {
			rot_spd_ = 1.7f - weight_;
			clothesState_ = ClothesState::BEGIN_WIND; },
			&dNumber_
		);
	}
	else if (message == EventMessage::NECK_SHOOT || message == EventMessage::NECK_BACK_SHOOT) {
		isDrawRange_ = true;
	}
}

void Clothes::SetPointsUpdate()
{
	collisionPoints.clear();

	if (laneNum_ != world_->GetKeepDatas().playerLane_ || !isDraw_) return;

	//ワールドマトリクス
	Matrix mat = Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0.0f));

	auto p1
		= Matrix::CreateTranslation(localPoints_[cuttingState_][0]) * mat;
	auto p2
		= Matrix::CreateTranslation(localPoints_[cuttingState_][1]) * mat;
	auto p3
		= Matrix::CreateTranslation(localPoints_[cuttingState_][2]) * mat;
	auto p4
		= Matrix::CreateTranslation(localPoints_[cuttingState_][3]) * mat;

	collisionPoints.push_back(Vector2(p1.Translation().x, p1.Translation().y));
	collisionPoints.push_back(Vector2(p2.Translation().x, p2.Translation().y));
	collisionPoints.push_back(Vector2(p3.Translation().x, p3.Translation().y));
	collisionPoints.push_back(Vector2(p4.Translation().x, p4.Translation().y));
}

bool Clothes::GetIsWind() const {
	return isWind_;
}

Vector2 Clothes::GetFulcrum() const {
	return fulcrum_;
}

std::vector<Vector2> Clothes::GetCollisionPoints() const {
	return collisionPoints;
}

void Clothes::Pendulum(Vector2 fulcrum, float length)
{
	beforePos_ = position_;

	//現在の重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//重力移動量を反映した重りの位置
	auto length_vec = position_ - fulcrum;
	auto t = -(length_vec.y * GRAVITY) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto gx = position_.x + t * length_vec.x;
	auto gy = position_.y + GRAVITY + t * length_vec.y;

	//2つの重りの位置の角度差
	auto r = MathHelper::ATan(gy - fulcrum.y, gx - fulcrum.x);

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

	//摩擦
	rot_ *= friction_;

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//角度を画像に反映
	angle_ = rot_ - 90.0f;
	
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
		Pendulum(fulcrum_, LENGTH);

		switch (clothesState_)
		{
		case ClothesState::BEGIN_STRONG_WIND: {
			rot_spd_ = 4.0f - weight_;
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
			friction_ = 1.0f;
			isFriction_ = false;
			count_ = 0;
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

void Clothes::UpdateClothesFeces()
{
	if (clothesFeces_ != nullptr && isDroping_ && cuttingState_ == Normal)
		clothesFeces_->Update();
}

void Clothes::DrawClothesFeces() const
{
	if (clothesFeces_ != nullptr && isDroping_ && cuttingState_ == Normal)
		clothesFeces_->Draw();
}

void Clothes::DrawRangeUpdate()
{
	if (player_->GetIsBiteMode())
		isDrawRange_ = false;
}

void Clothes::Synchronize()
{
	if (parent_ == nullptr || player_== nullptr || !player_->GetIsBiteMode()) return;

	pendulumVec_ = (position_ - beforePos_);
	auto pos = parent_->GetPosition() + pendulumVec_;
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0.0f)));
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

void Clothes::DrawRange() const
{
	if (currentStage_ != Stage::Stage1 || 
		collisionPoints.empty() ||
		!isDrawRange_ ||
		isWind_ ||
		isDroping_) return;

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
		modiPos[i][0] = drawP[i] + Vector2(-topSize.x / 2.0f, 0.0f);
		modiPos[i][1] = drawP[i] + Vector2(topSize.x / 2.0f, 0.0f);
		modiPos[i][2] = drawP[i] + Vector2(0.0f, -topSize.x / 2.0f);
		modiPos[i][3] = drawP[i] + Vector2(0.0f, topSize.x / 2.0f);
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

void Clothes::DrawHangerRange(Vector2 startPos, Vector2 endPos) const
{
	if (currentStage_ != Stage::Stage1 ||
		!isDrawRange_ ||
		player_->GetLaneNum() != laneNum_) return;

	auto topSize = Sprite::GetInstance().GetSize(SPRITE_ID::BITE_RANGE_TOP_SPRITE);
	auto origin = Sprite::GetInstance().GetSize(SPRITE_ID::BITE_RANGE_SPRITE) / 2;
	auto topOrigin = topSize / 2;

	Vector2 drawP[2];
	drawP[0] = GetDrawPosVect(startPos);
	drawP[1] = GetDrawPosVect(endPos);
	Vector2 modiPos[2][2];
	for (int i = 0; i < 2; i++) {
		//Pointから見て、左、右、上、下
		modiPos[i][0] = drawP[i] + Vector2(0.0f, -topSize.x / 2.0f);
		modiPos[i][1] = drawP[i] + Vector2(0.0f, topSize.x / 2.0f);
	}

	auto handle = Sprite::GetInstance().GetHandle(SPRITE_ID::BITE_RANGE_SPRITE);
	DrawModiGraph(modiPos[0][0].x, modiPos[0][0].y, modiPos[1][0].x, modiPos[1][0].y,
		modiPos[1][1].x, modiPos[1][1].y, modiPos[0][1].x, modiPos[0][1].y, handle, true);

	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[0], topOrigin, Vector2::One, angle_);
	Sprite::GetInstance().Draw(SPRITE_ID::BITE_RANGE_TOP_SPRITE, drawP[1], topOrigin, Vector2::One, angle_);
}
