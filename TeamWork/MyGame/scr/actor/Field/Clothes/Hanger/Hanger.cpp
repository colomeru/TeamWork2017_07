#include "Hanger.h"
#include "../MyGame/scr/actor/player/Player_Head.h"
#include "../MyGame/scr/game/Random.h"

Hanger::Hanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::HANGER;
	//parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(200, 200.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));
	parameter_.ClothSegmentPoints_.push_back(Vector2(-100.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2(0.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2(100.f, 100.f));

	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);
	colFuncMap_[COL_ID::BOX_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

Hanger::~Hanger()
{
}

void Hanger::Update()
{
	if (parent_ == nullptr || player_ == nullptr) return;
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);
	}
	//Hanger‚ð‚Â‚©‚ñ‚¾Û‚ÌPlayer‚ÌˆÚ“®ˆ—
	if (player_->GetIsBiteMode()) {
		velocity_ = Vector2(10.0f, 0.0f);
		Vector2 pos = parent_->GetPosition() + velocity_;
		player_->setCurPHeadSPos(pos);
		position_ += velocity_;
	}
	isHit_ = false;
}

void Hanger::Draw() const
{
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;
	//¶ã,‰Eã,¶‰º,‰E‰º
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(0, 0, 255), TRUE);
	//DrawLine(pos.x - seg.x, pos.y - seg.y, pos.x + seg.x, pos.y + seg.y, GetColor(255, 255, 255));

	DrawFormatString(700, 80, GetColor(255, 255, 255), "pos x:%f y:%f", parameter_.mat.Translation().x, parameter_.mat.Translation().y);

	if (parent_ == nullptr) return;
	DrawFormatString(700, 100, GetColor(255, 255, 255), "pos x:%f y:%f", parent_->GetPosition().x, parent_->GetPosition().y);
	DrawFormatString(700, 120, GetColor(255, 255, 255), "pos x:%f y:%f", parent_->GetPrevPosition().x, parent_->GetPrevPosition().y);
	DrawFormatString(700, 140, GetColor(255, 255, 255), "pos x:%f y:%f", 
		parent_->GetPosition().x - parent_->GetPrevPosition().x, parent_->GetPosition().y - parent_->GetPrevPosition().y);
}

void Hanger::OnUpdate()
{
}

void Hanger::OnCollide(Actor & other, CollisionParameter colpara)
{
	switch (colpara.colID)
	{
	case COL_ID::BOX_BOX_COL:
	{
		auto player = static_cast<Player*>(other.GetParent());
		if (player->GetIsBiteMode()) {
			parent_ = static_cast<Player_Head*>(&other);
			player_ = player;
		}
		break;
	}
	case COL_ID::BOX_CLOTHES_COL:
	{
		auto size = (parameter_.size.x + other.GetParameter().size.x) / 2;
		position_.x = other.GetPosition().x - size;
		player_->SetMode(4);
		parent_ = nullptr;
		player_ = nullptr;
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
		basePosition_ = position_;
		isPendulum_ = true;
		break;
	}
	}
}

