#include "Hairball.h"

Hairball::Hairball(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::HAIRBALL;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50, 50.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

Hairball::~Hairball()
{
}

void Hairball::Update()
{
	if (player_Head_) return;
	
	velocity_ = Vector2(-10.0f, 0.0f);

	position_ += velocity_;

	if (position_.x < player_->GetPosition().x - 400)
		parameter_.isDead = true;
}

void Hairball::Draw() const
{
}

void Hairball::OnUpdate()
{
}

void Hairball::OnCollide(Actor & other, CollisionParameter colpara)
{
	player_ = static_cast<Player*>(&other);
	player_Head_ = static_cast<Player_Head*>(&other);
}

void Hairball::OnMessage(EventMessage message, void * param)
{
}
