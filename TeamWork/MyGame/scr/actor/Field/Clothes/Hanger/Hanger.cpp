#include "Hanger.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

Hanger::Hanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	,isStop_(false), isSlip_(false)
{
	clothes_ID = CLOTHES_ID::HANGER;
	parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.size = Vector2(150.0f, 80.0f);

	position_ = pos - Vector2(0.0f, LENGTH / 2.0f);
	velocity_ = Vector2(10.0f, 0.0f);

	colFuncMap_[COL_ID::BOX_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

Hanger::~Hanger()
{
}

void Hanger::Update()
{
	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		parent_ = nullptr;
		return;
	}
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);
	}

	if (isStop_) return;

	//Hanger‚ð‚Â‚©‚ñ‚¾Û‚ÌPlayer‚ÌˆÚ“®ˆ—
	Vector2 pos = parent_->GetPosition() + velocity_;
	player_->setCurPHeadSPos(pos);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0.0f)));
	position_ += velocity_;
}

void Hanger::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, parameter_.size.y);
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, drawPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	Vector2 startPos = drawPos - Vector2(parameter_.size.x / 2.0f, 0.0f);
	Vector2 endPos = drawPos + Vector2(parameter_.size.x / 2.0f, 0.0f);
	DebugDraw::DebugDrawCircle(startPos.x, startPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(endPos.x, endPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(startPos.x, startPos.y, endPos.x, endPos.y, GetColor(255, 255, 255));
}

void Hanger::OnCollide(Actor & other, CollisionParameter colpara)
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
		break;
	}
	case ACTOR_ID::STAGE_ACTOR:
	{
		if (position_.x >= other.GetPosition().x) return;
		isStop_ = true;
		if (player_->GetRotBack() > 70.0f || isSlip_) return;
		player_->SetMode(MODE_SLIP);
		isSlip_ = true;
		parent_ = nullptr;
		break;
	}
	default:
		break;
	}
}

void Hanger::OnMessage(EventMessage message, void * param)
{
}

