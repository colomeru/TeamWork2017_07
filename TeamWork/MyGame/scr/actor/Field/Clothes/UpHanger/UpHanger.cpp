#include "UpHanger.h"
#include "../MyGame/scr/actor/player/Player_Head.h"
#include "../../../../tween/TweenManager.h"

const float DESTINATION = -600.0f;

UpHanger::UpHanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	, isMove_(false), isPull_(false)
{
	clothes_ID = CLOTHES_ID::FLUFFY_CLOTHES;
	parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.size = Vector2(150.f, 80.f);
	parameter_.radius = 12.0f;

	laneNum_ = laneNum;

	position_ = pos - Vector2(0, LENGTH / 2);
	startPos_ = position_;
	velocity_ = Vector2(0.0f, -5.0f);

	codePos_[0] = position_ - Vector2(parameter_.size.x / 2.0f + 3.0f, 5.0f);
	codePos_[1] = position_ - Vector2(parameter_.size.x / 2.0f + 3.0f, 0.0f);
	codeCenterPos_ = position_ + Vector2(0.0f, -2.5f);
	codePos_[2] = codeCenterPos_ + Vector2(0.0f, 2.5f);
	codePos_[3] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	codePos_[4] = position_ + Vector2(parameter_.size.x / 2.0f - 6.0f, 0.0f);
	codePos_[5] = position_ + Vector2(parameter_.size.x / 2.0f - 6.0f, -5.0f);
}

UpHanger::~UpHanger()
{
}

void UpHanger::Update()
{
	if (isMove_) {
		codePos_[2] = codeCenterPos_ + Vector2(0.0f, 2.5f);
		codePos_[3] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	}
	else return;

	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		parent_ = nullptr;
		return;
	}

	if (isPull_) {
		Spring(codeCenterPos_, 1.0f, 0.5f, 0.8f);
	}
	player_->setCurPHeadSPos(pHeadPos_);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));	
}

void UpHanger::Draw() const
{
	Vector2 drawPos = GetDrawPosVect(position_);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::UPHANGER_SPRITE).x / 2, parameter_.size.y);
	Sprite::GetInstance().Draw(SPRITE_ID::UPHANGER_SPRITE, drawPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	auto handle = Sprite::GetInstance().GetHandle(SPRITE_ID::UPHANGER_CODE_SPRITE);
	auto drawP0 = GetDrawPosVect(codePos_[0]);
	auto drawP1 = GetDrawPosVect(codePos_[1]);
	auto drawP2 = GetDrawPosVect(codePos_[2]);
	auto drawP3 = GetDrawPosVect(codePos_[3]);
	auto drawP4 = GetDrawPosVect(codePos_[4]);
	auto drawP5 = GetDrawPosVect(codePos_[5]);
	DrawModiGraph(drawP0.x, drawP0.y, drawP1.x, drawP1.y, drawP2.x, drawP2.y, drawP3.x, drawP3.y, handle, true);
	DrawModiGraph(drawP3.x, drawP3.y, drawP2.x, drawP2.y, drawP4.x, drawP4.y, drawP5.x, drawP5.y, handle, true);

	Vector2 startPos = drawPos - Vector2(parameter_.size.x / 2, 0);
	Vector2 endPos = drawPos + Vector2(parameter_.size.x / 2, 0);
	DebugDraw::DebugDrawCircle(startPos.x, startPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(endPos.x, endPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(startPos.x, startPos.y, endPos.x, endPos.y, GetColor(255, 255, 255));
}

void UpHanger::OnCollide(Actor & other, CollisionParameter colpara)
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
		player_->SetUseKey(false);
		isMove_ = true;
		isPull_ = true;
		auto baseCenter = codeCenterPos_;
		pHeadPos_ = parent_->GetPosition();
		codeCenterPos_ = colpara.colPos;
		auto pullPos = position_ + Vector2(0, 150.0f);
		TweenManager::GetInstance().Add(EaseInCubic, &codeCenterPos_, pullPos, 1.0f, [=]() {
			isPull_ = false;
			TweenManager::GetInstance().Add(EaseOutBounce, &codeCenterPos_.y, baseCenter.y, 0.5f);
			TweenManager::GetInstance().Add(EaseInOutSine, &pHeadPos_.y, DESTINATION, 0.5f, [=]() {
				world_->ChangeCamMoveMode(-1);
				player_->SetLaneNum(laneNum_);
				player_->SetUseKey(true);
				isMove_ = false;
			});
		});
		break;
	}
	}
}

void UpHanger::OnMessage(EventMessage message, void * param)
{
}

void UpHanger::Spring(const Vector2& pos, float stiffnes, float friction, float mass)
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
