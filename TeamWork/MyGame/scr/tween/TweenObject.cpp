#include "TweenObject.h"
#include "../math/MathHelper.h"

TweenObject::TweenObject(float* value, float b, float c, float d, const std::function<void()>& callback, float s) :
	value_(value),
	b_(b), c_(c), d_(d), s_(s),
	callback_(callback),
	timer_(0.0f),
	timeScale_(1.0f),
	isEnd_(false),
	loopType_(UpdateType::Common),
	loopCount_(0),
	sine_(1.0f)
{
}

TweenObject::~TweenObject()
{
	value_ = nullptr;
	callback_ = nullptr;
}

void TweenObject::Update(const float deltaTime)
{
	if (!isEnd_)
		updateFunc_(deltaTime);
}

bool TweenObject::IsEnd() const
{
	return isEnd_;
}

void TweenObject::End()
{
	isEnd_ = true;
	callback_ = nullptr;
}

void TweenObject::Play(float scale)
{
	timeScale_ = scale;
}

void TweenObject::Stop()
{
	timeScale_ = 0.0f;
}

float * TweenObject::GetValuePointer() const
{
	return value_;
}

void TweenObject::SetFunction(const TweenFuncParam& func)
{
	func_ = func;
}

void TweenObject::SetLoopType(const UpdateType type)
{
	switch (type)
	{
	case UpdateType::Common:   updateFunc_ = std::function<void(float)>([=](float deltaTime) { CommonUpdate(deltaTime); }); break;
	case UpdateType::Loop:	   updateFunc_ = std::function<void(float)>([=](float deltaTime) { LoopUpdate(deltaTime); }); break;
	case UpdateType::PingPong: updateFunc_ = std::function<void(float)>([=](float deltaTime) { PingPongUpdate(deltaTime); }); break;
	}
}

void TweenObject::SetLoopCount(int count)
{
	loopCount_ = count;
}

void TweenObject::CommonUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	if (value_ != nullptr)
		*value_ = func_(timer_, b_, c_, d_, s_);

	if (timer_ >= d_)
	{
		Invoke();
		isEnd_ = true;
		callback_ = nullptr;
	}
}

void TweenObject::LoopUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	if (value_ != nullptr)
		*value_ = func_(timer_, b_, c_, d_, s_);

	if (timer_ >= d_)
	{
		// 初期値と移動量を反転
		b_ += c_;
		c_ *= -1.0f;

		timer_ = 0.0f;
		sine_ *= -1.0f;
		// 行きフラグ | sine_ < 0 < sine_  | 帰りフラグ
		if (sine_ > 0.0f)
		{
			Invoke();
			loopCount_--;
			loopCount_ = MathHelper::Max(loopCount_, -1);

			// ループ終了
			if (loopCount_ == 0)
				isEnd_ = true;
		}
	}
}

void TweenObject::PingPongUpdate(float deltaTime)
{
	timer_ += deltaTime * timeScale_ * sine_;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	if (value_ != nullptr)
		*value_ = func_(timer_, b_, c_, d_, s_);

	if (timer_ >= d_)
	{
		sine_ *= -1.0f;
	}
	else if (timer_ <= 0.0f)
	{
		sine_ *= -1.0f;

		Invoke();
		loopCount_--;
		loopCount_ = MathHelper::Max(loopCount_, -1);

		// ループ終了
		if (loopCount_ == 0)
			isEnd_ = true;
	}
}

void TweenObject::Invoke()
{
	if (callback_ != nullptr)
		callback_();
}
