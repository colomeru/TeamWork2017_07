#include "FadePanel.h"
#include "../graphic/Sprite.h"
#include "../math/MathHelper.h"

FadePanel::FadePanel() :
color_(ColorType::Black),
alpha_(1.0f),
maxAlpha_(1.0f),
actionTime_(0.0f),
inTime_(1.0f),
outTime_(1.0f),
delayTime_(0.0f)
{
	// ステートリセット
	while (!stateStack_.empty())
	{
		stateStack_.pop();
	}
	stateStack_.push(FADE_STATUS::eStandby);
}

FadePanel::~FadePanel()
{
}

void FadePanel::Initialize()
{
	/* 初期設定 */
	color_		= ColorType::Black;
	alpha_		= 1.0f;
	maxAlpha_	= 1.0f;
	actionTime_	= 0;
	inTime_		= 1.0f;
	outTime_	= 1.0f;
	delayTime_  = 0.0f;

	// ステートリセット
	while (!stateStack_.empty())
	{
		stateStack_.pop();
	}
	stateStack_.push(FADE_STATUS::eStandby);
}

void FadePanel::Update(float deltaTime)
{
	// 変異中は省略
	if (!IsAction()) return;
	  
	// ディレイ省略
	if (delayTime_ > 0.0f)
	{
		delayTime_ -= deltaTime;
		delayTime_  = MathHelper::Max(0.0f, delayTime_);
		return;
	}

	switch (stateStack_.top())
	{
	// フェードイン
	case FADE_STATUS::eFadeIn:	FadeInUpdate(deltaTime);	break;
	case FADE_STATUS::eFadeOut:	FadeOutUpdate(deltaTime);	break;
	}

	// アルファクランプ
	alpha_ = MathHelper::Clamp(alpha_, 0.0f, maxAlpha_);
}

void FadePanel::Draw()const
{
	switch (color_)
	{
	case FadePanel::White:
		//Sprite::GetInstance().Draw(SPRITE_ID::WHITE_SCREEN_SPRITE, Vector2::Zero, Vector2::Zero, alpha_, SCALE, 0.0f, false, false);
		break;
	case FadePanel::Black:
		//Sprite::GetInstance().Draw(SPRITE_ID::BLACK_SCREEN_SPRITE, Vector2::Zero, Vector2::Zero, alpha_, SCALE, 0.0f, false, false);
		break;
	}
}

// フェードイン開始
void FadePanel::FadeIn()
{
	stateStack_.push(FADE_STATUS::eFadeIn);
	actionTime_ = GetInTime();
}

// フェードアウト開始
void FadePanel::FadeOut(ColorType type, float maxAlpha)
{
	stateStack_.push(FADE_STATUS::eFadeOut);
	color_	= type;
	maxAlpha_	= maxAlpha;
	actionTime_ = GetOutTime();
}

// 変異中か？
bool FadePanel::IsAction() const
{
	return stateStack_.top() != FADE_STATUS::eStandby;
}

// 画面が埋まっているか？
bool FadePanel::IsFillScreen() const
{
	return alpha_ >= maxAlpha_;
}

// 画面が透明か？
bool FadePanel::IsClearScreen() const
{
	return alpha_ <= 0.0f;
}

// フェードインタイムの取得
float FadePanel::GetInTime() const
{
	return inTime_;
}

// フェードインタイムの設定
void FadePanel::SetInTime(const float sec, const float delay)
{
	inTime_	   = sec;
	delayTime_ = delay;
}

// フェードアウトタイムの取得
float FadePanel::GetOutTime() const
{
	return outTime_;
}

// フェードアウトタイムの設定
void FadePanel::SetOutTime(const float sec, const float delay)
{
	outTime_   = sec;
	delayTime_ = delay;
}

// ディレイタイム取得
float FadePanel::GetDelayTime() const
{
	return delayTime_;
}

void FadePanel::AddCollBack(std::function<void()> collback)
{
	callbacks_.push_back(collback);
}

// フェードインアップデート
void FadePanel::FadeInUpdate(float deltaTime)
{
	alpha_ -= (deltaTime / actionTime_) * (maxAlpha_ / 1.0f);

	// 終了判定
	if (IsClearScreen())
	{
		stateStack_.pop();
		
		// コールバック呼び出し
		for (auto& callback : callbacks_)
			callback();
		callbacks_.clear();
	}
}

// フェードアウトアップデート
void FadePanel::FadeOutUpdate(float deltaTime)
{
	alpha_ += (deltaTime / actionTime_) * (maxAlpha_ / 1.0f);

	// 終了判定
	if (IsFillScreen())
	{
		stateStack_.pop();

		// コールバック呼び出し
		for (auto& callback : callbacks_)
			callback();
		callbacks_.clear();
	}
}
