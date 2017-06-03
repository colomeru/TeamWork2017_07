#include "ClothesTapper.h"
#include"../../../../game/Random.h"
#include"../../../../tween/TweenManager.h"
#include"../../../../math/Easing.h"

static const float moveTime = 2.0f;
//static const float TapTime = 2.0f;
static const float IdleTime = 1.0f;

ClothesTapper::ClothesTapper(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), timeCount_(0.f),targetPos_(pos),basePos_(pos),updateMode_(MODE_IDLE), spriteID_(SPRITE_ID::CLOTHES_TAP_01_SPRITE)
{
	//parameter_.radius = Sprite::GetInstance().GetSize(spriteID_).x/2;
	parameter_.radius = 100.f;

	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_01_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_02_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_03_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_04_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_05_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_06_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_07_SPRITE);
	anmManager_.Add(SPRITE_ID::CLOTHES_TAP_08_SPRITE);

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
	anmManager_.Draw(drawPos_ + Vector2(0, 300.f), origin,Vector2(2.0f,2.0f));
	//Sprite::GetInstance().Draw(spriteID_, drawPos_+Vector2(0,400), origin,Vector2(2.f,2.f));
	
	if (BuildMode != 1)return;

	DrawCircle(drawPos_.x, drawPos_.y, parameter_.radius, GetColor(255, 255, 255));

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
	
	Vector2 moveVec = targetPos_ - basePos_;
	//d=かかる秒数
	TweenManager::GetInstance().Add(&position_,EaseInOutQuad, basePos_, moveVec, moveTime);

	

}

void ClothesTapper::ToIdleMode()
{
	updateMode_ = MODE_IDLE;
	timeCount_ = 0.0f;
	anmManager_.ResetAnm();
}

void ClothesTapper::MoveUpdate()
{
	timeCount_ += 0.016f;

	//ここの更新はMoveModeになるときに1度だけ移動量指定をする(現在移動先を指定するFT系は、ループしないもののみ利用可能
	//Vector2 movePos = targetPos_ - basePos_;
	//Vector3 d3pos = Easing::EaseInOutQuad(timeCount_, Vector3(basePos_), Vector3(movePos), moveTime);
	//position_ = Vector2(d3pos.x, d3pos.y);
	
	if (timeCount_ > moveTime) {
		ToTapMode();
	}
}

void ClothesTapper::TapUpdate()
{
	timeCount_ += 0.016f;
	anmManager_.Update();
	//if (timeCount_ > TapTime) {
	if (timeCount_ > anmManager_.GetAnmEndTime()) {
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
