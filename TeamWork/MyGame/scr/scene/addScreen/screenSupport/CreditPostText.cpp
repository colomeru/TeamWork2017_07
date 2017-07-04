#include "CreditPostText.h"
#include "../../../tween/TweenManager.h"
#include "../../../actor/player/Player_Head.h"
#include "../../../game/ID.h"

CreditPostText::CreditPostText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame) :
	Clothes(world, id, laneNum, 0.0f), f2(0.0f)
{
	spriteId_ = sprite;
	frame_ = frame;
	laneNum_ = world_->GetKeepDatas().playerLane_;
	position_ = Vector2(position.x, position.y);
	parameter_.size = size;
	parameter_.radius = 32.0f;
	fulcrum_ = Vector2(position_.x, position_.y - parameter_.size.y / 2.0f);
	auto toX = -400 - 1070;
	TweenManager::GetInstance().Add(Linear, &position_, Vector2(toX, position_.y), 10.3f, [=]() {Dead(); });
	TweenManager::GetInstance().Add(Linear, &fulcrum_, Vector2(toX, fulcrum_.y), 10.3f);
	colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	localPoints.push_back(Vector3(-parameter_.size.x / 2.0f, 0.0f, 0.0f));
	localPoints.push_back(Vector3(-parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints.push_back(Vector3(parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints.push_back(Vector3(parameter_.size.x / 2.0f, 0.0f, 0.0f));
	SetPointsUpdate();

	//�J���X
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);
}

CreditPostText::~CreditPostText()
{
}

void CreditPostText::Update()
{
	anmManager_.Update();

	velocity_ = Vector2(position_.x - f2, 0.0f);
	f2 = position_.x;

	SetPointsUpdate();

	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		parent_ = nullptr;
		return;
	}

	Vector2 pos = parent_->GetPosition() + velocity_;
	player_->setCurPHeadSPos(pos);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));
}

void CreditPostText::Draw() const
{
	Sprite& ins = Sprite::GetInstance();
	auto drawPos = GetDrawPosVect(position_);
	ins.Draw(SPRITE_ID::CREDIT_TOWEL_SPRITE, Vector2(drawPos.x - parameter_.size.x / 2.0f - 20, drawPos.y), Vector2(0, parameter_.size.y / 2.0f), Vector2(parameter_.size.x / 600.0f + 0.1f, 1.5f));
	ins.Draw(spriteId_, Vector2(drawPos.x, drawPos.y), Vector2(parameter_.size.x / 2.0f, parameter_.size.y / 2.0f), Vector2::One);

	//�J���X
	Vector2 origin = ins.GetSize(SPRITE_ID::BIRD_SPRITE);
	Vector2 middle = Vector2(drawPos.x + 55, drawPos.y - 233);
	Vector2 right = Vector2(middle.x + parameter_.size.x / 2.5f - 20, middle.y);
	Vector2 left = Vector2(middle.x - parameter_.size.x / 2.5f - 20, middle.y);
	Vector2 corr = Vector2(-63.0f, -19.0f);
	ins.Draw(SPRITE_ID::CREDIT_CHAIN_SPRITE, middle + corr);
	ins.Draw(SPRITE_ID::CREDIT_CHAIN_SPRITE, right + corr);
	ins.Draw(SPRITE_ID::CREDIT_CHAIN_SPRITE, left + corr);
	anmManager_.Draw(middle, origin, Vector2::One, 1.0f);
	anmManager_.Draw(right, origin, Vector2::One, 1.0f);
	anmManager_.Draw(left, origin, Vector2::One, 1.0f);


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

void CreditPostText::OnCollide(Actor & other, CollisionParameter colpara)
{
	parent_ = &other;
	static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
	player_ = static_cast<CreditPlayer*>(parent_->GetParent());
	player_->CurHeadBite(other.GetPosition());
	player_->SetIsBiteMode(true);
}
