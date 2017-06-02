#include "Hairball.h"
#include "../MyGame/scr/Def.h"

Hairball::Hairball(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::HAIRBALL;
	parameter_.ID = ACTOR_ID::HAIRBALL_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50, 50.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	world_->EachActor(ACTOR_ID::PLAYER_HEAD_ACTOR, [&, this](const Actor& other) {
		player_Head_ = static_cast<Player_Head*>(const_cast<Actor*>(&other));
	});
	player_ = static_cast<Player*>(player_Head_->GetParent());

	localPoints.push_back(Vector3());
	localPoints.push_back(Vector3());
	localPoints.push_back(Vector3());
	localPoints.push_back(Vector3());

	SetPointsUpdate();

	colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);

	isHit_ = false;

}

Hairball::~Hairball()
{
}

void Hairball::Update()
{
	if (player_ == nullptr) return;
	
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::BOX_BOX_COL);
	}

	velocity_ = Vector2(-10.0f, 0.0f);

	position_ += velocity_;
	
	if (position_.x < player_->GetPosition().x - 400)
		parameter_.isDead = true;
}

void Hairball::Draw() const
{
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::HAIRBALL_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::HAIRBALL_SPRITE, drawPos_, crcOrigin, spriteAlpha_, Vector2::One, angle_);
}

void Hairball::OnUpdate()
{
}

void Hairball::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (other.GetParameter().ID)
	{
	case ACTOR_ID::PLAYER_HEAD_ACTOR: {
		player_Head_ = static_cast<Player_Head*>(const_cast<Actor*>(&other));
		if (!player_Head_->getIsCurrentHead()) return;

		if (player_->GetIsBiteMode()) {
			player_->SetMode(MODE_SLIP);
			player_Head_->setIsBiteSlipWind(true);
			parameter_.isDead = true;
		}
		break;
	}
	case ACTOR_ID::PLAYER_SWORD_ACTOR: {
		parameter_.isDead = true;
		break;
	}
	}
}

void Hairball::OnMessage(EventMessage message, void * param)
{
}
