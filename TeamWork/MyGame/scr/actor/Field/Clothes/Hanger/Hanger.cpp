#include "Hanger.h"
#include "../MyGame/scr/actor/player/Player_Head.h"
#include "../MyGame/scr/game/Random.h"

Hanger::Hanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	,isStop_(false)
{
	clothes_ID = CLOTHES_ID::HANGER;
	parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(150, 80.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));
	parameter_.ClothSegmentPoints_.push_back(Vector2(-100.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2(0.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2(100.f, 100.f));

	laneNum_ = laneNum;

	position_ = pos - Vector2(0, length_ / 2);
	fulcrum_ = pos - Vector2(0, length_);

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

	//HangerÇÇ¬Ç©ÇÒÇæç€ÇÃPlayerÇÃà⁄ìÆèàóù
	velocity_ = Vector2(10.0f, 0.0f);
	Vector2 pos = parent_->GetPosition() + velocity_;
	player_->setCurPHeadSPos(pos);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));
	position_ += velocity_;

	isHit_ = false;
}

void Hanger::Draw() const
{
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 4));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;
	//ç∂è„,âEè„,ç∂â∫,âEâ∫
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);

	auto drawPos = GetDrawPosVect(position_);
	Vector2 hangOrigin = Vector2(Sprite::GetInstance().GetSize(SPRITE_ID::HANGER_SPRITE).x / 2, parameter_.size.y);
	Sprite::GetInstance().Draw(SPRITE_ID::HANGER_SPRITE, drawPos, hangOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	if (BuildMode != 1) return;
	DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

}

void Hanger::OnUpdate()
{
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
		//if (isStop_) return;
		if (position_.x >= other.GetPosition().x) return;
		isStop_ = true;
		if (player_->GetRot() > 80) return;
		player_->SetMode(MODE_SLIP);
		//player_->PHeadChanger();
		//static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(true);
		parent_ = nullptr;
		break;
	}
	default:
		break;
	}
}

void Hanger::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
	{
		if (!isUpdate_ || isPendulum_) break;
		int rand = Random::GetInstance().Range(0, 100);
		if (rand > 30) return;
		isPendulum_ = true;
		break;
	}
	}
}

