#include "Actor.h"
#include "../collision/Collision.h"
#include "../collision/CollisionDX.h"
#include "../conv/DXConverter.h"
#include "../graphic/Model.h"
#include"../collision/MyCol.h"
#include"../math/MyFuncionList.h"

Actor::Actor(IWorld * world, Actor * parent) :
	world_(world),
	colFunc_(world),
	parent_(parent),
	angle_(0.f),
	position_(Vector2::Zero),
	laneNum_(0),
	isDraw_(true),
	isUpdate_(true),
	drawAddPos_(Vector2::Zero)
{
	parameter_.spriteAlpha_=1.f;
}

Actor::~Actor()
{

}

void Actor::Collide(COL_ID id, Actor & other)
{
	if (laneNum_ != other.GetLaneNum())return;
	CollisionParameter colpara = colFuncMap_[id](*this, other);
	colpara.colID = id;
	if (colpara.colFrag)
	{
		OnCollide(other, colpara);
		other.OnCollide(*this, colpara);
	}
}

ActorParameter Actor::GetParameter() const
{
	return parameter_;
}

Matrix Actor::GetPose() const
{
	return Matrix(parameter_.mat.RotationMatrix())
		* Matrix::CreateTranslation(parameter_.mat.Translation());
}

void Actor::SetPose(const Matrix& mat)
{
	parameter_.mat = mat;
	position_ = Vector2(parameter_.mat.Translation().x, parameter_.mat.Translation().y);
}

void Actor::OnUpdate()
{
}

void Actor::LateComUpdate() {
	Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
	drawPos_ = GetDrawPosVect(position_);

	int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
	if(drawPos_.x>= -WINDOW_WIDTH/2.f&&drawPos_.x<=WINDOW_WIDTH*1.5f){
		isDraw_ = true;
		isUpdate_ = true;

		drawLane = MathHelper::Abs(drawLane);

		{
			auto pos = GetDrawPosVect(position_);
			Vector2 pdPos = GetFreeActorDrawPos(world_->GetKeepDatas().playerPos_,world_->GetKeepDatas().playerLane_);
			float toplayerlength = pos.y- pdPos.y;
			toplayerlength = abs(toplayerlength);
			float addalpha=1-toplayerlength / 500;
			addalpha /= 4;
			addalpha = MathHelper::Clamp(addalpha, 0.0f, 0.5f);
			addalpha += 0.75f;
			if (laneNum_ == world_->GetKeepDatas().playerLane_)addalpha = 1.f;
			parameter_.spriteAlpha_ = addalpha;
		}
	}
	else {
		isDraw_ = false;
		isUpdate_ = false;
	}

	isCheckCol_ = (world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[2] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[3])
		&& laneNum_ == world_->GetKeepDatas().playerLane_;
	if (world_->isChangeFrame()) {

	}
}

// 自分取得
Actor* Actor::GetActor() const
{
	return const_cast<Actor*>(this);
}

Actor* Actor::GetParent() const
{
	return parent_;
}

// メッセージ処理
void Actor::handleMessage(EventMessage message, void * param)
{
	OnMessage(message, param);
}

void Actor::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Actor::OnMessage(EventMessage message, void * param)
{
}
