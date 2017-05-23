#include "GoalClothes.h"
#include "../MyGame/scr/actor/UI/GoalUI.h"

GoalClothes::GoalClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::GOAL_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(200, 200.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	isHit_ = false;

	localPoints[0] = Vector3(-60, 0 + length_, 0);
	localPoints[1] = Vector3(-60, 100 + length_, 0);
	localPoints[2] = Vector3(60, 100 + length_, 0);
	localPoints[3] = Vector3(60, 0 + length_, 0);

	SetPointsUpdate();

	//colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

GoalClothes::~GoalClothes()
{
}

void GoalClothes::Update()
{
	SetPointsUpdate();

	if (player_ == nullptr || parent_ == nullptr)return;

	if (player_->GetIsBiteMode() && !isHit_) {
		world_->sendMessage(EventMessage::GOAL_FLAG);
		isHit_ = true;
	}
}

void GoalClothes::Draw() const
{
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

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
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 0, 255), TRUE);

	auto drawP1 = GetDrawPosVect(collisionPoints[0]);
	auto drawP2 = GetDrawPosVect(collisionPoints[1]);
	auto drawP3 = GetDrawPosVect(collisionPoints[2]);
	auto drawP4 = GetDrawPosVect(collisionPoints[3]);
	DrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
	DrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
	DrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
	DrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
	DrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
}

void GoalClothes::OnUpdate()
{
	
}

void GoalClothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	if (!isWind_) {
		parent_ = &other;
		player_ = static_cast<Player*>(parent_->GetParent());
		static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
		static_cast<Player*>(parent_->GetParent())->CurHeadBite(other.GetPosition());
		static_cast<Player*>(parent_->GetParent())->SetOtherClothesID_(clothes_ID);
	}
}

void GoalClothes::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::GOAL_FLAG:
		world_->Add(ACTOR_ID::UI_ACTOR, std::make_shared<GoalUI>(world_, position_));
		break;
	default:
		break;
	}
}

