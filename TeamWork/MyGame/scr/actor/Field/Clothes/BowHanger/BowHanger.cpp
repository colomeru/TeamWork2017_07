#include "BowHanger.h"
#include "../../../player/Player_Head.h"
#include "../../../../tween/TweenManager.h"
#include "../../../../graphic/Sprite.h"
#include "../../../../sound/sound.h"
#include <DxLib.h>

const float DESTINATION = -400.0f;

BowHanger::BowHanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	, isMove_(false), isPull_(false)
{
	clothes_ID = CLOTHES_ID::FLUFFY_CLOTHES;
	parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.size = Vector2(150.f, 80.f);
	parameter_.radius = 12.0f;

	position_ = pos - Vector2(0, LENGTH / 2.0f - 10.0f);

	codeCenterPos_ = position_;
	codePos_[0] = position_ + Vector2(-parameter_.size.x / 2.0f + 1.0f, -2.5f);
	codePos_[1] = position_ + Vector2(-parameter_.size.x / 2.0f + 1.0f, 2.5f);
	codePos_[2] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	codePos_[3] = codeCenterPos_ + Vector2(0.0f, 2.5f);
	codePos_[4] = position_ + Vector2(parameter_.size.x / 2.0f - 1.0f, -2.5f);
	codePos_[5] = position_ + Vector2(parameter_.size.x / 2.0f - 1.0f, 2.5f);
	baseCenter_ = codeCenterPos_;
}

BowHanger::~BowHanger()
{
}

void BowHanger::Update()
{
	DrawRangeUpdate();

	if (!isMove_) return;

	codePos_[2] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	codePos_[3] = codeCenterPos_ + Vector2(0.0f, 2.5f);

	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		Cancel();
		return;
	}

	if (isPull_) {
		Spring(codeCenterPos_, 1.0f, 0.5f, 0.8f);
	}
	player_->setCurPHeadSPos(pHeadPos_);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0.0f)));
}

void BowHanger::Draw() const
{
	Vector2 drawPos = GetDrawPosVect(position_);
	auto handle = Sprite::GetInstance().GetHandle(SPRITE_ID::BOWHANGER_CODE_SPRITE);
	Vector2 drawP[6];
	for (int i = 0; i < 6; i++) {
		drawP[i] = GetDrawPosVect(codePos_[i]);
	}
	DrawModiGraph(drawP[0].x, drawP[0].y, drawP[2].x, drawP[2].y, drawP[3].x, drawP[3].y, drawP[1].x, drawP[1].y, handle, true);
	DrawModiGraph(drawP[2].x, drawP[2].y, drawP[4].x, drawP[4].y, drawP[5].x, drawP[5].y, drawP[3].x, drawP[3].y, handle, true);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::BOWHANGER_SPRITE).x / 2, parameter_.size.y);
	Sprite::GetInstance().Draw(SPRITE_ID::BOWHANGER_SPRITE, drawPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	DrawHangerRange(codePos_[0], codePos_[4]);
}

void BowHanger::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (other.GetParameter().ID)
	{
	case ACTOR_ID::PLAYER_HEAD_ACTOR:
	{
		parent_ = &other;
		static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
		player_ = static_cast<Player*>(parent_->GetParent());
		player_->CurHeadBite(other.GetPosition());
		player_->SetIsBiteMode(true);
		player_->SetOtherClothesID_(clothes_ID);
		pHeadPos_ = parent_->GetPosition();
		codeCenterPos_ = colpara.colPos;
		isMove_ = true;
		isPull_ = true;
		TweenManager::GetInstance().Delay(2.0f, [=]() {
			if (parent_ == nullptr) {
				Sound::GetInstance().StopSE(SE_ID::BELLOWS_SE);
				return;
			}
			//紐の引っ張り
			Sound::GetInstance().PlaySE(SE_ID::BELLOWS_SE);
			auto toSpringPos = position_ + (player_->GetPosition() - position_).Normalize() * 100.0f;
			TweenManager::GetInstance().Add(EaseOutQuad, &codeCenterPos_, toSpringPos, 1.0f, [=]() {
				//飛ばした後の紐の揺れ
				Sound::GetInstance().StopSE(SE_ID::BELLOWS_SE);
				Sound::GetInstance().PlaySE(SE_ID::BLOWOFF_SE);
				TweenManager::GetInstance().Add(EaseOutBounce, &codeCenterPos_, baseCenter_, 0.4f, [=]() {isMove_ = false;});
				isPull_ = false;
				Shoot();
			});
		});
		break;
	}
	}
}

void BowHanger::OnMessage(EventMessage message, void * param)
{
}

void BowHanger::Spring(const Vector2& pos, float stiffnes, float friction, float mass)
{
	//バネの伸び具合を計算
	const auto stretch = pHeadPos_ - pos;
	//バネの力を計算
	const auto force = -stiffnes * stretch;
	//加速度を追加
	const auto accelerration = force / mass;
	//移動速度を計算
	velocity_ = friction * (velocity_ + accelerration);
	//座標の更新
	pHeadPos_ += velocity_;
}

void BowHanger::Cancel()
{
	TweenManager& instance = TweenManager::GetInstance();
	instance.Cancel(&codeCenterPos_);
	instance.Add(EaseOutBounce, &codeCenterPos_, baseCenter_, 0.4f, [=]() {
		isMove_ = false;
		isPull_ = false;
	});
	parent_ = nullptr;
}

void BowHanger::Shoot()
{
	if (parent_ == nullptr) return;
	static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(true);
	player_->SetIsBiteMode(false);
	auto toPos = (position_ - parent_->GetPosition()).Normalize();
	toPos.x = toPos.x * player_->GetCurrentHeadLength().x / 8.0f;
	toPos.x = MathHelper::Min(toPos.x, 80.0f);
	toPos.y = toPos.y * 40.0f;
	player_->SetPendulumVect(toPos);
	player_->SetMode(MODE_SLIP);
	parent_ = nullptr;
}
