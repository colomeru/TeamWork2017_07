#include "MenuCrow.h"
#include "../../../tween/TweenManager.h"
#include "../../../Def.h"
#include "../../../time/Time.h"
#include "../../../graphic/Sprite.h"
#include "../../../debugdata/DebugDraw.h"

//コンストラクタ
MenuCrow::MenuCrow() :
	position_(Vector2::Zero), from_(Vector2(0.0f, 0.0f)), interval_(0.0f), timer_(0.0f), velocity_(Vector2(-5.0f, 0.0f)), dis_(0.0f), state_(State::Idle)
{
	//アニメーション読み込み
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
	//アニメーション更新
	anmManager_.Update();

	//状態に合わせた更新
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
	//パネルに合わせて縦移動する
	dis_ += betDis;
	TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
}

//初期化
void MenuCrow::Initialize(Vector2 position, float interval)
{
	position_ = position;
	interval_ = interval;
}

//移動
void MenuCrow::Move()
{
	position_ += velocity_;

	if (position_.x <= -OffSet) {//画面外に出たら待機位置に戻る
		timer_ = 0.0f;
		position_.x = WINDOW_WIDTH + OffSet;
		state_ = State::Idle;
	}
}

//待機
void MenuCrow::Idle()
{
	timer_ += Time::DeltaTime;
	from_ = Vector2(0.0f, 0.0f);
	if (timer_ >= interval_) { //待機時間後に移動開
		//縦移動リセット
		dis_ = 0.0f;
		from_ = 0.0f;
		state_ = State::Move;
	}
}
