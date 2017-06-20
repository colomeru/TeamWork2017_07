#include "CreditText.h"
#include "../../../tween/TweenManager.h"
#include "../../../actor/player/Player_Head.h"
#include "../../../game/ID.h"

CreditText::CreditText(IWorld* world, CLOTHES_ID id,int laneNum, Vector2 position) :
	Clothes(world, id, laneNum, 0.0f)
{
	laneNum_=world_->GetKeepDatas().playerLane_;
	position_ = position;
	parameter_.size = Vector2(800, 200);
	parameter_.radius = 32.0f;
	//TweenManager::GetInstance().Add(Linear, &position_, Vector2(-parameter_.size.x, position_.y), 5.0f, [=]() {Dead(); });
	//TweenManager::GetInstance().Add(Linear, &fulcrum_, Vector2(-parameter_.size.x, fulcrum_.y), 5.0f);
	colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	fulcrum_ = Vector2(position_.x, position_.y - parameter_.size.y / 2.0f);
	localPoints.push_back(Vector3(-parameter_.size.x / 2.0f, 0.0f, 0.0f));
	localPoints.push_back(Vector3(-parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints.push_back(Vector3(parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints.push_back(Vector3(parameter_.size.x / 2.0f, 0.0f, 0.0f));
	SetPointsUpdate();
}

CreditText::~CreditText()
{
}

void CreditText::Update()
{
	velocity_ = Vector2(-10.0f, 0.0f);
	position_ += velocity_;
	fulcrum_ += velocity_;
	SetPointsUpdate();

	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		parent_ = nullptr;
		return;
	}

	if (isCheckCol_ && isUpdate_) {
		//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::BOX_BOX_COL);
	}


	Vector2 pos = parent_->GetPosition() + velocity_;
	player_->setCurPHeadSPos(pos);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));

}

void CreditText::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);

	auto min = drawPos - Vector2(parameter_.size.x / 2.0f, parameter_.size.y / 2.0f);
	auto max = drawPos + Vector2(parameter_.size.x / 2.0f, parameter_.size.y / 2.0f);
	DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255, 0), 1);

	if (BuildMode != 1) return;
	if (!collisionPoints.empty()) {
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

}

void CreditText::OnCollide(Actor & other, CollisionParameter colpara)
{
	parent_ = &other;
	static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
	player_ = static_cast<CreditPlayer*>(parent_->GetParent());
	player_->CurHeadBite(other.GetPosition());
	player_->SetIsBiteMode(true);
}
