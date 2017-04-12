#include "Actor.h"
#include "../collision/Collision.h"
#include "../collision/CollisionDX.h"
#include "../conv/DXConverter.h"
#include "../graphic/Model.h"

Actor::Actor(IWorld * world, Actor * parent) :
world_(world),
parent_(parent)
{
	colFunc_[COL_ID::TEST_COL] = std::bind(&Actor::Test_Col, this, std::placeholders::_1);
}

Actor::~Actor()
{
}

void Actor::Collide(COL_ID id, Actor & other)
{
	CollisionParameter colpara = colFunc_[id](other);
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
	position_ = parameter_.mat.Translation();
}

void Actor::OnUpdate()
{
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

CollisionParameter Actor::Test_Col(const Actor & other) const
{
	CollisionParameter colpara;
	colpara.colID = COL_ID::TEST_COL;
	colpara.colFrag = CollisionDX::Capsule_vs_Capsule(Capsule(GetParameter()), Capsule(other.GetParameter()));

	return colpara;
}
