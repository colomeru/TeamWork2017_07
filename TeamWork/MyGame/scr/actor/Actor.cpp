#include "Actor.h"
#include "../collision/Collision.h"
#include "../collision/CollisionDX.h"
#include "../conv/DXConverter.h"
#include "../graphic/Model.h"
#include"../collision/MyCol.h"
#include"../math/MyFuncionList.h"

Actor::Actor(IWorld * world, Actor * parent) :
	world_(world),
	colFunc_(world),
	parent_(parent),
	angle_(0.f),
	position_(Vector2::Zero),
	laneNum_(0),
	isDraw_(true),
	isUpdate_(true),
	drawAddPos_(Vector2::Zero)
{
	parameter_.spriteAlpha_=1.f;
}

Actor::~Actor()
{

}

void Actor::Collide(COL_ID id, Actor & other)
{
	if (laneNum_ != other.GetLaneNum())return;
	CollisionParameter colpara = colFuncMap_[id](*this, other);
	colpara.colID = id;
	if (colpara.colFrag)
	{
		OnCollide(other, colpara);
		other.OnCollide(*this, colpara);
	}
}

ActorParameter Actor::GetParameter() const
{
	return parameter_;
}

Matrix Actor::GetPose() const
{
	return Matrix(parameter_.mat.RotationMatrix())
		* Matrix::CreateTranslation(parameter_.mat.Translation());
}

void Actor::SetPose(const Matrix& mat)
{
	parameter_.mat = mat;
	position_ = Vector2(parameter_.mat.Translation().x, parameter_.mat.Translation().y);
}

void Actor::OnUpdate()
{
}

void Actor::UpdateList() {
	if (!isUpdate_)return;
	Update();
}

void Actor::FastUpdate() {
}

Vector2 Actor::GetDrawAddPos() const {
	return drawAddPos_;
}

void Actor::FastComUpdate() {
	drawAddPos_ = Vector2::Zero;
	if (world_->isChangeFrame()) {
		isUpdate_ = true;
	}
	else {
		isUpdate_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
	}
}

void Actor::LateUpdate() {

}

void Actor::CommonUpdate() {
	prevPosition_ = position_;
}

void Actor::LateComUpdate() {
	Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
	drawPos_ = GetDrawPosVect(position_);

	int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
	if (drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f) {
		isDraw_ = true;
		isUpdate_ = true;

		drawLane = MathHelper::Abs(drawLane);

		{
			auto pos = GetDrawPosVect(position_);
			Vector2 pdPos = GetFreeActorDrawPos(world_->GetKeepDatas().playerPos_, world_->GetKeepDatas().playerLane_);
			float toplayerlength = pos.y - pdPos.y;
			toplayerlength = abs(toplayerlength);
			float addalpha = 1 - toplayerlength / 500;
			addalpha /= 4;
			addalpha = MathHelper::Clamp(addalpha, 0.0f, 0.5f);
			addalpha += 0.75f;
			if (laneNum_ == world_->GetKeepDatas().playerLane_)addalpha = 1.f;
			parameter_.spriteAlpha_ = addalpha;
		}
	}
	else {
		isDraw_ = false;
		isUpdate_ = false;
	}

	isCheckCol_ = (world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[2] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[3])
		&& laneNum_ == world_->GetKeepDatas().playerLane_;
	if (world_->isChangeFrame()) {

	}
}

//レーン移動時限定のアップデート、virtualだが、Player以外はoverrideしないようにする事

bool Actor::CamMoveUpdate() {
	CamMoveOnlyUpdate();
	parameter_.spriteAlpha_ = 0.5f;
	if (0 > world_->GetKeepDatas().nextLane_) {
		CamMoveUp();
	}
	else {
		CamMoveDown();
	}
	drawPos_ = GetDrawPosVect(position_);
	return true;
}

void Actor::CamMoveOnlyUpdate() {}

void Actor::StartOnlyUpdate() {

}

void Actor::StartOnlyLateUpdate() {

}

bool Actor::StartModeUpdate() {
	StartOnlyUpdate();
	isDraw_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
	isUpdate_ = true;

	StartOnlyLateUpdate();
	return false;
}

void Actor::CamMoveUp() {
	float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
	laneLerpNum = min(1.f, laneLerpNum);
	int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
	drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum + 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
}

void Actor::CamMoveDown() {
	LaneChangeFall();
}

Vector2 Actor::GetVelocity() const {
	return velocity_;
}

Vector2 Actor::GetDrawPosVect(const Vector2 & pos) const {
	Vector2 retPos;

	Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
	retPos = Vector2(cmpos3d.x, cmpos3d.y);

	int drawLane;
	drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;

	if (world_->GetIsFreeCamY_() && drawLane < 0) {

		retPos.y += defDrawLinePosY[drawLane + 1] * 1.2f;

		return retPos;
	}

	if (MathHelper::Abs(drawLane) >= 2) {
		retPos.y = -500;
	}
	else {
		retPos.y += defDrawLinePosY[drawLane + 1];

	}
	retPos += drawAddPos_;
	return retPos;
}

//lanenumを自由に指定してdrawposを算出

Vector2 Actor::GetFreeActorDrawPos(const Vector2 & pos, int laneNum) const {
	Vector2 retPos;

	Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
	retPos = Vector2(cmpos3d.x, cmpos3d.y);

	int drawLane;
	drawLane = laneNum - world_->GetKeepDatas().playerLane_;

	if (world_->GetIsFreeCamY_() && drawLane < 0) {

		retPos.y += defDrawLinePosY[drawLane + 1] * 1.2f;

		return retPos;
	}

	if (MathHelper::Abs(drawLane) >= 2) {
		retPos.y = -500;
	}
	else {
		retPos.y += defDrawLinePosY[drawLane + 1];

	}
	retPos += drawAddPos_;
	return retPos;
}

// 自分取得
Actor* Actor::GetActor() const
{
	return const_cast<Actor*>(this);
}

Actor* Actor::GetParent() const
{
	return parent_;
}

Vector2 Actor::GetPosition() const {

	return position_;
}

Vector2 Actor::GetDrawPos() const {
	return drawPos_;
}

Vector2 Actor::GetPrevPosition() const {
	return prevPosition_;
}

float Actor::GetAngle() const {
	return angle_;
}

int Actor::GetLaneNum() const {
	return laneNum_;
}

bool Actor::GetIsUpdate() const {
	return isUpdate_;
}

bool Actor::GetIsDraw() const {
	return isDraw_;
}

void Actor::SetLaneNum(int laneNum) {
	laneNum_ = laneNum;
}

// メッセージ処理
void Actor::handleMessage(EventMessage message, void * param)
{
	OnMessage(message, param);
}

void Actor::DrawUpdate() {
	drawPos_ = GetDrawPosVect(position_);
}

void Actor::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Actor::OnMessage(EventMessage message, void * param)
{
}

//落ちる時のレーン変更

void Actor::LaneChangeFall() {
	float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
	laneLerpNum = min(1.f, laneLerpNum);
	int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
	drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
}
