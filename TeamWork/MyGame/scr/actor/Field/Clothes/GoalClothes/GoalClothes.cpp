#include "GoalClothes.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/actor/UI/GoalUI.h"
#include "../../../player/Player_Head.h"

GoalClothes::GoalClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
{
	clothes_ID = CLOTHES_ID::GOAL_CLOTHES;
	parameter_.ID = ACTOR_ID::GOAL_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(200.0f, 1500.0f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, length_);

	isHit_ = false;
}

GoalClothes::~GoalClothes()
{
}

void GoalClothes::Update()
{
	if (player_ == nullptr || parent_ == nullptr)return;

	if (player_->GetIsBiteMode() && !isHit_) {
		world_->sendMessage(EventMessage::GOAL_FLAG);
		isHit_ = true;
	}
}

void GoalClothes::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	//DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 0, 255), TRUE);
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::GOAL_CLOTHES_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::GOAL_CLOTHES_SPRITE, drawPos - Vector2(0, 200), crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	if (BuildMode != 1) return;
	//if (!collisionPoints.empty()) {
	//	auto drawP1 = GetDrawPosVect(collisionPoints[0]);
	//	auto drawP2 = GetDrawPosVect(collisionPoints[1]);
	//	auto drawP3 = GetDrawPosVect(collisionPoints[2]);
	//	auto drawP4 = GetDrawPosVect(collisionPoints[3]);
	//	DrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
	//	DrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
	//	DrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
	//	DrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
	//	DrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
	//	DrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
	//	DrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
	//}
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//ç∂è„,âEè„,ç∂â∫,âEâ∫
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));
}

void GoalClothes::OnUpdate()
{
	
}

void GoalClothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	if (other.GetParameter().ID != ACTOR_ID::PLAYER_HEAD_ACTOR) return;
	if (!isWind_) {
		parent_ = &other;
		static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
		player_ = static_cast<Player*>(const_cast<Actor*>(parent_->GetParent()));
		player_->CurHeadBite(other.GetPosition());
		player_->SetIsBiteMode(true);
		player_->SetOtherClothesID_(clothes_ID);
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

