#include "ClothesTapper.h"
#include"../../../../game/Random.h"
#include"../../../../tween/TweenManager.h"
#include"../../../../math/Easing.h"

static const float moveTime = 2.0f;
static const float TapTime = 2.0f;
static const float IdleTime = 1.0f;

ClothesTapper::ClothesTapper(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), timeCount_(0.f),targetPos_(pos),basePos_(pos),updateMode_(MODE_IDLE), spriteID_(SPRITE_ID::TEST2_SPRITE)
{
	parameter_.radius = Sprite::GetInstance().GetSize(spriteID_).x/2;

	//auto ePx = std::make_shared<EaseNode>(&position_.x, EaseType::Linear, 0.f, -200.f, 5.f, [this]() {SetNextTapPos(); });
	////auto ePy = std::make_shared<EaseNode>(&position_.y, EaseType::Linear, position_.y, 5.f, 5.f, [this]() {SetNextTapPos(); });
	//EasingManager::GetInstance().Add(ePx);
	////EasingManager::GetInstance().Add(ePy);
	updateFunctionMap_[MODE_MOVE] = std::bind(&ClothesTapper::MoveUpdate, this);
	updateFunctionMap_[MODE_TAP] = std::bind(&ClothesTapper::TapUpdate, this);
	updateFunctionMap_[MODE_IDLE] = std::bind(&ClothesTapper::IdleUpdate, this);

	colFuncMap_[COL_ID::TAPPER_PHEAD_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

ClothesTapper::~ClothesTapper()
{
}

void ClothesTapper::Update()
{
	updateFunctionMap_[updateMode_]();
}

void ClothesTapper::Draw() const
{
	auto origin = Sprite::GetInstance().GetSize(spriteID_) / 2;
	Sprite::GetInstance().Draw(spriteID_, drawPos_, origin,Vector2::One);
}

void ClothesTapper::OnUpdate()
{
}

void ClothesTapper::OnCollide(Actor & other, CollisionParameter colpara)
{
	auto pH = static_cast<Player_Head*>(&other);

	if (pH->getIsCurrentHead() && pH->GetPlayerPointer()->GetIsBiteMode()) {
		pH->GetPlayerPointer()->SetMode(MODE_SLIP);
	}
}

void ClothesTapper::OnMessage(EventMessage message, void * param)
{
}

void ClothesTapper::SetNextTapPos(const Vector2 & pos)
{
	basePos_ = position_;
	timeCount_ = 0.f;

	targetPos_ = Vector2(world_->GetKeepDatas().playerPos_.x, 0.f);
	targetPos_.x += Random::GetInstance().Range(0.f, WINDOW_WIDTH / 10.f)*5.f;

	if (pos.x != 0.0f&&pos.y!=0.0f) {
		targetPos_ = pos;
	}
}

void ClothesTapper::PlayTap()
{
	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::TAPPER_PHEAD_COL);
}

void ClothesTapper::ToTapMode()
{
	updateMode_ = MODE_TAP;
	timeCount_ = 0.0f;

}

void ClothesTapper::ToMoveMode()
{
	updateMode_ = MODE_MOVE;
	timeCount_ = 0.0f;
	int laneTNum = world_->GetKeepDatas().playerLane_ - laneNum_;
	position_.y -= defDrawLinePosY[1]- defDrawLinePosY[1-sign(laneTNum)];
	laneNum_ = world_->GetKeepDatas().playerLane_;
	SetNextTapPos();
	
	Vector2 moveVec = targetPos_ - position_;

	//d=かかる秒数、
	TweenManager::GetInstance().Add(&position_,EaseOutQuad, position_, moveVec, moveTime/2.0f);

	

	//Vector3 d3pos = Easing::EaseInOutQuadFT(timeCount_, moveTime, basePos_, targetPos_);
	//position_ = Vector2(d3pos.x, d3pos.y);

}

void ClothesTapper::ToIdleMode()
{
	updateMode_ = MODE_IDLE;
	timeCount_ = 0.0f;
}

void ClothesTapper::MoveUpdate()
{
	timeCount_ += 0.016f;

	//ここの更新はMoveModeになるときに1度だけ移動量指定をする(現在移動先を指定するFT系は、ループしないもののみ利用可能
	//Vector3 d3pos = Easing::EaseInOutQuadFT(timeCount_, moveTime, basePos_, targetPos_);
	//position_ = Vector2(d3pos.x, d3pos.y);
	
	if (timeCount_ > moveTime) {
		ToTapMode();
	}
}

void ClothesTapper::TapUpdate()
{
	timeCount_ += 0.016f;

	if (timeCount_ > TapTime) {
		PlayTap();
		ToIdleMode();
	}

}

void ClothesTapper::IdleUpdate()
{
	timeCount_ += 0.016f;

	if (timeCount_ > IdleTime) {
		ToMoveMode();
	}

}
