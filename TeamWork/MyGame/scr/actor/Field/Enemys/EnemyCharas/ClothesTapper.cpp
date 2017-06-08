#include "ClothesTapper.h"
#include"../../../../game/Random.h"
#include"../../../../tween/TweenManager.h"
#include"../../../../math/Easing.h"
#include"../../../../input/Keyboard.h"
#include"../../../Effects/EnemyEffect/TapperTapEffect.h"

static const float moveTime = 2.0f;
//static const float TapTime = 2.0f;
static const float IdleTime = 3.0f;
static const Vector2 shiftPos=Vector2(-200.f, 200.f);

ClothesTapper::ClothesTapper(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), timeCount_(0.f),targetPos_(pos),basePos_(pos),updateMode_(MODE_IDLE), spriteID_(SPRITE_ID::CLOTHES_TAP_01_SPRITE), anmManager_(), shiftPos_(shiftPos)
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
	
	//anmManager_.ReverseAnm();
	
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_01_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_02_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_03_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_04_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_05_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_06_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_07_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_08_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_09_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_10_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_11_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_12_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_13_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_14_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_15_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_16_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_17_SPRITE);
	anmManager2_.Add(SPRITE_ID::CLOTHES_TAP_DEAD_18_SPRITE);

	anmManager2_.SetAnmSpeed(8);
	//auto ePx = std::make_shared<EaseNode>(&position_.x, EaseType::Linear, 0.f, -200.f, 5.f, [this]() {SetNextTapPos(); });
	////auto ePy = std::make_shared<EaseNode>(&position_.y, EaseType::Linear, position_.y, 5.f, 5.f, [this]() {SetNextTapPos(); });
	//EasingManager::GetInstance().Add(ePx);
	////EasingManager::GetInstance().Add(ePy);
	updateFunctionMap_[MODE_MOVE] = std::bind(&ClothesTapper::MoveUpdate, this);
	updateFunctionMap_[MODE_TAP] = std::bind(&ClothesTapper::TapUpdate, this);
	updateFunctionMap_[MODE_IDLE] = std::bind(&ClothesTapper::IdleUpdate, this);
	updateFunctionMap_[MODE_DEAD] = std::bind(&ClothesTapper::DeadUpdate, this);

	colFuncMap_[COL_ID::TAPPER_PHEAD_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);
	colFuncMap_[COL_ID::TAPPER_PSWORD_COL] = std::bind(&CollisionFunction::IsHit_Tapper_PSword, colFunc_, std::placeholders::_1, std::placeholders::_2);
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
	if (updateMode_ == MODE_DEAD)anmManager2_.Draw(drawPos_ + Vector2(0, 300.f), origin, Vector2(2.0f, 2.0f));
	else anmManager_.Draw(drawPos_ + Vector2(0, 300.f), origin,Vector2(2.0f,2.0f));
	//Sprite::GetInstance().Draw(spriteID_, drawPos_+Vector2(0,400), origin,Vector2(2.f,2.f));
	
	if (BuildMode != 1)return;

	DrawCircle(drawPos_.x+GetShiftPos().x, drawPos_.y+ GetShiftPos().y, parameter_.radius, GetColor(255, 255, 255));

}

void ClothesTapper::OnUpdate()
{
}

void ClothesTapper::OnCollide(Actor & other, CollisionParameter colpara)
{
	if (colpara.colID == COL_ID::TAPPER_PHEAD_COL) {
		auto pH = static_cast<Player_Head*>(&other);

		if (pH->getIsCurrentHead() && pH->GetPlayerPointer()->GetIsBiteMode()) {
			pH->GetPlayerPointer()->SetMode(MODE_SLIP);
		}
	}
	else if (colpara.colID == COL_ID::TAPPER_PSWORD_COL) {
		ToDeadMode();
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
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<TapperTapEffect>(world_, position_));
	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::TAPPER_PHEAD_COL);
}

void ClothesTapper::ToTapMode()
{
	updateMode_ = MODE_TAP;
	timeCount_ = 0.0f;

	PlayTap();
	anmManager_.ResetAnm();
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

	//anmManager_.ResetAnm();
	anmManager_.ReverseAnm();

	shiftPos_ -= shiftPos;
}

void ClothesTapper::ToIdleMode()
{
	updateMode_ = MODE_IDLE;
	timeCount_ = 0.0f;

	shiftPos_ += shiftPos;
}

void ClothesTapper::ToDeadMode()
{
	updateMode_ = MODE_DEAD;
	//anmManager_.ReverseAnm();
	shiftPos_ -= shiftPos;
}

void ClothesTapper::MoveUpdate()
{
	timeCount_ += 0.016f;

	anmManager_.Update();
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
	if (anmManager_.IsEndAnimation()) {
		//PlayTap();
		ToIdleMode();
	}

}

void ClothesTapper::IdleUpdate()
{
	timeCount_ += 0.016f;

	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_SWORD_ACTOR, COL_ID::TAPPER_PSWORD_COL);
	
	if (BuildMode==1&&Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Z))ToDeadMode();

	if (timeCount_ > IdleTime) {
		ToMoveMode();
	}

}

void ClothesTapper::DeadUpdate()
{
	anmManager2_.Update();
	if (anmManager2_.IsEndAnimation()) {
		parameter_.isDead = true;
		world_->sendMessage(EventMessage::TAPPER_DEAD);
	}
}
