#include "ThinClothes.h"
#include "../MyGame/scr/game/Random.h"
#include "../../ClothesPin.h"

ThinClothes::ThinClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::THIN_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 16.0f;
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

	localPoints[0] = Vector3(-60, 0 + length_, 0);
	localPoints[1] = Vector3(-60, 100 + length_, 0);
	localPoints[2] = Vector3(60, 100 + length_, 0);
	localPoints[3] = Vector3(60, 0 + length_, 0);

	SetPointsUpdate();

	//if (is_Pin)
	//	world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum_, Vector2(100, 100), this, fulcrum_));

	//colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

ThinClothes::~ThinClothes()
{
}

void ThinClothes::Update()
{
	if (parent_ != nullptr) {
		if (!static_cast<Player*>(parent_)->GetIsBiteMode()) {
			parent_ = nullptr;
		}
	}

	ShakesClothes();
	WindSwing();

	SetPointsUpdate();

	//if (isCheckCol_ && isUpdate_) {
	//	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::BOX_BOX_COL);
	//}

	isHit_ = false;
}

void ThinClothes::Draw() const
{
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;
	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(204, 204, 204), TRUE);

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

	//DrawLine(pos.x - seg.x, pos.y - seg.y, pos.x + seg.x, pos.y + seg.y, GetColor(255, 255, 255));
}

void ThinClothes::OnUpdate()
{
}

void ThinClothes::OnMessage(EventMessage message, void * param)
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
