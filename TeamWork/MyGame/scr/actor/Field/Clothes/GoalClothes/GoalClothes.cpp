#include "GoalClothes.h"
#include "../../../UI/GoalUI.h"
#include "../../../player/Player_Head.h"
#include "TutorialFlash.h"
#include "../../../../graphic/Sprite.h"
#include "../../../../debugdata/DebugDraw.h"

GoalClothes::GoalClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	,isHit_(false)
	,isFlash_(false)
{
	clothes_ID = CLOTHES_ID::GOAL_CLOTHES;
	parameter_.ID = ACTOR_ID::GOAL_ACTOR;
	parameter_.radius = 16.0f;
	parameter_.size = Vector2(220.0f, 1500.0f);

	laneNum_ = laneNum;

	position_ = pos;
	fulcrum_ = position_ - Vector2(0, LENGTH);

	tutorialFlash_ = std::make_shared<TutorialFlash>(world, laneNum_, position_, this->GetActor());
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
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::GOAL_CLOTHES_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::GOAL_CLOTHES_SPRITE, drawPos - Vector2(0, 200), crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle_);

	if (isFlash_) {
		tutorialFlash_->Draw();
	}

	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2.0f), (parameter_.size.y / 2.0f));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//ç∂è„,âEè„,ç∂â∫,âEâ∫
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	DebugDraw::DebugDrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));
}

void GoalClothes::OnCollide(Actor & other, CollisionParameter colpara)
{
	if (other.GetParameter().ID != ACTOR_ID::PLAYER_HEAD_ACTOR) return;
	if (!isWind_) {
		isFlash_ = false;
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
	case EventMessage::TUTORIAL_GOAL_FLASH:
		isFlash_ = true;
		break;
	default:
		break;
	}
}

