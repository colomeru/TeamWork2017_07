#include "MenuCrow.h"
#include "../../../tween/TweenManager.h"
#include "../../../Def.h"
#include "../../../time/Time.h"
#include "../../../graphic/Sprite.h"

//コンストラクタ
MenuCrow::MenuCrow(Vector2 position, float interval) :
	position_(position), from_(Vector2(0.0f, 0.0f)), interval_(interval), timer_(0.0f), velocity_(Vector2(-5.0f, 0.0f)), dis_(0.0f)
{
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);
}

//デストラクタ
MenuCrow::~MenuCrow()
{
}

//更新
void MenuCrow::Update(const int stageNum)
{
	anmManager_.Update();

	//if (position_.x <= -OffSet) {//画面外に出たら
	//	timer_ = 0.0f;
	//	position_.x = WINDOW_WIDTH + OffSet;
	//}
	//if (position_.x == WINDOW_WIDTH + OffSet) {
	//	if (stageNum >= 7) return;
	//	timer_ += Time::DeltaTime;
	//}
	//if (timer_ > interval_) {
	//	position_ += velocity_;
	//}
	//else {
	//	dis_ = 0.0f;
	//	from_ = 0.0f;
	//}
	switch (state_)
	{
	case State::Move:
		Move();
		break;
	case State::Idle:
		if (stageNum >= 7) return;
		Idle();
		break;
	}
}

//描画
void MenuCrow::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(position_ + from_, origin, Vector2::One, 1.0f);
}

//目的地加算
void MenuCrow::AddDistance(float betDis)
{
	if (timer_ <= interval_) return;
	dis_ += betDis;
	TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
}

void MenuCrow::Initialize(Vector2 position, float interval)
{
	position_ = position;
	interval_ = interval;
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);
	velocity_ = Vector2(-5.0f, 0.0f);
	state_ = State::Idle;
}

void MenuCrow::Move()
{
	position_ += velocity_;

	if (position_.x <= -OffSet) {//画面外に出たら
		timer_ = 0.0f;
		position_.x = WINDOW_WIDTH + OffSet;
		state_ = State::Idle;
	}

}

void MenuCrow::Idle()
{
	timer_ += Time::DeltaTime;
	dis_ = 0.0f;
	from_ = Vector2(0.0f, 0.0f);
	if (timer_ >= interval_) {
		state_ = State::Move;
	}
}
