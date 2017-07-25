#include "Hairball.h"
#include "../../../../tween/TweenManager.h"
#include "../../../../graphic/Sprite.h"

Hairball::Hairball(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	, player_Head_(nullptr)
{
	clothes_ID = CLOTHES_ID::HAIRBALL;
	parameter_.ID = ACTOR_ID::HAIRBALL_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50.0f, 50.0f);

	laneNum_ = laneNum;
	position_ = pos;
	velocity_ = Vector2(-10.0f, 0.0f);
	fulcrum_ = position_ - Vector2(0.0f, LENGTH);

	colFuncMap_[COL_ID::PHEAD_HAIRBALL_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);
	colFuncMap_[COL_ID::PSWORD_HAIRBALL_COL] = std::bind(&CollisionFunction::IsHit_Hairball_PSword, colFunc_, std::placeholders::_1, std::placeholders::_2);

	float vecY = position_.y + 150.0f;
	TweenManager::GetInstance().Delay(8.0f, [=]() {Dead(); });
	TweenManager::GetInstance().Loop(EaseInOutQuad, &position_.y, vecY, 1.5f);
}

Hairball::~Hairball()
{
	TweenManager::GetInstance().Cancel(&position_);
}

void Hairball::Update()
{
	if (player_ == nullptr) return;
	
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::PHEAD_HAIRBALL_COL);
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_SWORD_ACTOR, COL_ID::PSWORD_HAIRBALL_COL);
	}

	position_ += velocity_;

	angle_++;
}

void Hairball::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::HAIRBALL_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::HAIRBALL_SPRITE, drawPos, crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);
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
			Dead();
		}
		break;
	}
	case ACTOR_ID::PLAYER_SWORD_ACTOR: {
		Dead();
		break;
	}
	}
}

void Hairball::OnMessage(EventMessage message, void * param)
{
}
