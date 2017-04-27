#include "GoalClothes.h"
#include "../MyGame/scr/actor/UI/GoalUI.h"

GoalClothes::GoalClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::GOAL_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(200, 200.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;

	position_ = pos;

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [&, this](const Actor& other) {
		player_ = const_cast<Actor*>(&other);
	});

}

GoalClothes::~GoalClothes()
{
}

void GoalClothes::Update()
{
	if (laneNum_ == world_->GetKeepDatas().playerLane_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::BOX_BOX_COL);
	}
	if (laneNum_ == world_->GetKeepDatas().nextLane_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::BOX_BOX_COL);
	}

	float pPosX = player_->GetPosition().x;
	float gPosx = position_.x;

	if (pPosX > gPosx) {
		world_->sendMessage(EventMessage::GOAL_FLAG);
	}
	isHit_ = false;

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

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 255, 255), TRUE);

}

void GoalClothes::OnUpdate()
{
	
}

void GoalClothes::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void GoalClothes::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::GOAL_FLAG:
		world_->Add(ACTOR_ID::UI_ACTOR, std::make_shared<GoalUI>(world_, position_));
		parameter_.isDead = true;
		break;
	default:
		break;
	}
}
