#include "UpHanger.h"
#include "../../../player/Player_Head.h"
#include "../../../../tween/TweenManager.h"
#include "../../../../time/Time.h"
#include "../../../../debugdata/DebugDraw.h"
#include "../../../../graphic/Sprite.h"

const float DESTINATION = -400.0f;		//目標値

UpHanger::UpHanger(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum, 0.0f)
	, isMove_(false), isPull_(false)
{
	clothes_ID = CLOTHES_ID::FLUFFY_CLOTHES;
	parameter_.ID = ACTOR_ID::HANGER_ACTOR;
	parameter_.size = Vector2(150.0f, 80.0f);
	parameter_.radius = 12.0f;

	laneNum_ = laneNum;

	position_ = pos - Vector2(0, LENGTH / 2.0f);

	bonePos_[0] = position_ - Vector2(parameter_.size.x / 2.0f, parameter_.size.y);
	bonePos_[1] = position_ + Vector2(parameter_.size.x / 2.0f, -parameter_.size.y);

	codeCenterPos_ = position_;
	codePos_[0] = position_ + Vector2(-parameter_.size.x / 2.0f, -2.5f);
	codePos_[1] = position_ + Vector2(-parameter_.size.x / 2.0f, 2.5f);
	codePos_[2] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	codePos_[3] = codeCenterPos_ + Vector2(0.0f, 2.5f);
	codePos_[4] = position_ + Vector2(parameter_.size.x / 2.0f, -2.5f);
	codePos_[5] = position_ + Vector2(parameter_.size.x / 2.0f, 2.5f);
	baseCenter_ = codeCenterPos_;
}

UpHanger::~UpHanger()
{
	Cancel();
}

void UpHanger::Update()
{
	DrawRangeUpdate();

	if (!isMove_) return;

	codePos_[2] = codeCenterPos_ + Vector2(0.0f, -2.5f);
	codePos_[3] = codeCenterPos_ + Vector2(0.0f, 2.5f);

	if (parent_ == nullptr || player_ == nullptr) return;
	if (!player_->GetIsBiteMode()) {
		Cancel();
		return;
	}

	if (isPull_) {
		Spring(codeCenterPos_, 1.0f, 0.5f, 0.8f);
	}
	//頭の座標を更新
	player_->setCurPHeadSPos(pHeadPos_);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));	
}

void UpHanger::Draw() const
{
	Vector2 drawPos = GetDrawPosVect(position_);
	auto boneHandle = Sprite::GetInstance().GetHandle(SPRITE_ID::UPHANGER_SPRITE);
	Vector2 boneP[2];
	for (int i = 0; i < 2; i++) {
		boneP[i] = GetDrawPosVect(bonePos_[i]);
	}
	auto codeHandle = Sprite::GetInstance().GetHandle(SPRITE_ID::UPHANGER_CODE_SPRITE);
	Vector2 drawP[6];
	for (int i = 0; i < 6; i++) {
		drawP[i] = GetDrawPosVect(codePos_[i]);
	}
	DrawModiGraph(boneP[0].x, boneP[0].y, boneP[1].x, boneP[1].y, drawP[5].x, drawP[5].y, drawP[1].x, drawP[1].y, boneHandle, true);
	DrawModiGraph(drawP[0].x, drawP[0].y, drawP[2].x, drawP[2].y, drawP[3].x, drawP[3].y, drawP[1].x, drawP[1].y, codeHandle, true);
	DrawModiGraph(drawP[2].x, drawP[2].y, drawP[4].x, drawP[4].y, drawP[5].x, drawP[5].y, drawP[3].x, drawP[3].y, codeHandle, true);

	DrawHangerRange(codePos_[0], codePos_[4]);

	Vector2 startPos = drawPos - Vector2(parameter_.size.x / 2, 0);
	Vector2 endPos = drawPos + Vector2(parameter_.size.x / 2, 0);
	DebugDraw::DebugDrawCircle(startPos.x, startPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawCircle(endPos.x, endPos.y, parameter_.radius, GetColor(255, 255, 255));
	DebugDraw::DebugDrawLine(startPos.x, startPos.y, endPos.x, endPos.y, GetColor(255, 255, 255));
}

void UpHanger::OnCollide(Actor & other, CollisionParameter colpara)
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

		if (player_->GetPosition().y - player_->GetPrevPosition().y < 0.0f || laneNum_ == 0) return;
		codeCenterPos_ = colpara.colPos;
		UpPlayer();
		break;
	}
	}
}

void UpHanger::OnMessage(EventMessage message, void * param)
{
}

void UpHanger::Spring(const Vector2& pos, float stiffnes, float friction, float mass)
{
	//バネの伸び具合を計算
	const auto stretch = pHeadPos_ - pos;
	//バネの力を計算
	const auto force = -stiffnes * stretch;
	//加速度を追加
	const auto accelerration = force / mass;
	//移動速度を計算
	velocity_ = friction * (velocity_ + accelerration);
	//座標の更新
	pHeadPos_ += velocity_;
}

void UpHanger::Cancel()
{
	TweenManager::GetInstance().Cancel(&codeCenterPos_);
	TweenManager::GetInstance().Cancel(&pHeadPos_);
	player_->SetUseKey(true);
	isPull_ = false;
	isMove_ = false;
	parent_ = nullptr;
}

void UpHanger::UpPlayer()
{
	player_->SetUseKey(false);
	isMove_ = true;
	isPull_ = true;
	pHeadPos_ = parent_->GetPosition();
	auto pullPos = position_ + Vector2(0.0f, 150.0f);

	//プレイヤーの落下速度によってひもを引っ張る時間を変える
	float easeStart = Easing::EaseOutQuad(0.0f, codeCenterPos_.y, pullPos.y, 1.0f);
	float easeDelta = Easing::EaseOutQuad(Time::DeltaTime, codeCenterPos_.y, pullPos.y, 1.0f);
	float time = MathHelper::Clamp(MathHelper::Abs(player_->GetVelocity().y / (easeDelta - easeStart)), 0.5f, 1.0f);

	//移動前の位置を保存
	Vector2 baseCode1 = codePos_[1];
	Vector2 baseCode2 = codePos_[5];
	Vector2 deformCode1 = codePos_[1] + Vector2(5.0f, 8.0f);
	Vector2 deformCode2 = codePos_[5] + Vector2(-5.0f, 8.0f);

	//ハンガーのしなりと紐の引っ張り
	TweenManager::GetInstance().Add(EaseOutQuad, &codePos_[1], deformCode1, time);
	TweenManager::GetInstance().Add(EaseOutQuad, &codePos_[5], deformCode2, time);
	TweenManager::GetInstance().Add(EaseOutQuad, &codeCenterPos_, pullPos, time, [=]() {
		isPull_ = false;
		//ハンガーが元に戻るのとプレイヤーを飛ばす
		TweenManager::GetInstance().Add(EaseOutBounce, &codePos_[1], baseCode1, 0.4f);
		TweenManager::GetInstance().Add(EaseOutBounce, &codePos_[5], baseCode2, 0.4f);
		TweenManager::GetInstance().Add(EaseOutBounce, &codeCenterPos_.y, baseCenter_.y, 0.4f);
		TweenManager::GetInstance().Add(EaseInOutSine, &pHeadPos_.y, DESTINATION, 0.5f, [=]() {
			world_->ChangeCamMoveMode(-1);
			player_->SetLaneNum(laneNum_);
			player_->SetUseKey(true);
			isMove_ = false;
		});
	});

}
