#include "BowHanger.h"
#include "../MyGame/scr/actor/player/Player_Head.h"
#include "../../../../tween/TweenManager.h"

const float DESTINATION = -400.0f;

BowHanger::BowHanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
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

	Vector2 cor = Vector2(0.0f, 2.5f);
	codeCenterPos_[0] = position_ + Vector2(-parameter_.size.x / 2.0f, 0.0f);
	codePos_[0] = codeCenterPos_[0] - cor;
	codePos_[1] = codeCenterPos_[0] + cor;
	codeCenterPos_[1] = position_;
	codePos_[2] = codeCenterPos_[1] - cor;
	codePos_[3] = codeCenterPos_[1] + cor;
	codeCenterPos_[2] = position_ + Vector2(parameter_.size.x / 2.0f, 0.0f);
	codePos_[4] = codeCenterPos_[2] - cor;
	codePos_[5] = codeCenterPos_[2] + cor;
}

BowHanger::~BowHanger()
{
}

void BowHanger::Update()
{
	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		parent_ = nullptr;
		isMove_ = false;
		return;
	}

	if (player_->GetRotBack() < 90.0f)
		isMove_ = true;

	if (!isMove_) return;
	angle_ = player_->GetRotBack() - 90.0f;

	if (isPull_) {
		Spring(codeCenterPos_[1], 1.0f, 0.5f, 0.8f);
	}
	player_->setCurPHeadSPos(pHeadPos_);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));
}

void BowHanger::Draw() const
{
	Vector2 drawPos = GetDrawPosVect(position_);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, parameter_.size.y);
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, drawPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
	//auto handle = Sprite::GetInstance().GetHandle(SPRITE_ID::UPHANGER_CODE_SPRITE);
	//auto drawP0 = GetDrawPosVect(codePos_[0]);
	//auto drawP1 = GetDrawPosVect(codePos_[1]);
	//auto drawP2 = GetDrawPosVect(codePos_[2]);
	//auto drawP3 = GetDrawPosVect(codePos_[3]);
	//auto drawP4 = GetDrawPosVect(codePos_[4]);
	//auto drawP5 = GetDrawPosVect(codePos_[5]);
	//DrawModiGraph(drawP0.x, drawP0.y, drawP2.x, drawP2.y, drawP3.x, drawP3.y, drawP1.x, drawP1.y, handle, true);
	//DrawModiGraph(drawP2.x, drawP2.y, drawP4.x, drawP4.y, drawP5.x, drawP5.y, drawP3.x, drawP3.y, handle, true);

	//Vector2 startPos = drawPos - Vector2(parameter_.size.x / 2, 0);
	//Vector2 endPos = drawPos + Vector2(parameter_.size.x / 2, 0);
	//DebugDraw::DebugDrawCircle(startPos.x, startPos.y, parameter_.radius, GetColor(255, 255, 255));
	//DebugDraw::DebugDrawCircle(endPos.x, endPos.y, parameter_.radius, GetColor(255, 255, 255));
	//DebugDraw::DebugDrawLine(startPos.x, startPos.y, endPos.x, endPos.y, GetColor(255, 255, 255));
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
		//player_->SetUseKey(false);
		pHeadPos_ = parent_->GetPosition();
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

void BowHanger::RotatePoint()
{

}
