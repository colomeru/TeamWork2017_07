#include "TweenObject.h"
#include "../math/MathHelper.h"

TweenObject::TweenObject(float* value, float b, float c, float d, const std::function<void()>& callback, float s) :
value_(value),
b_(b), c_(c), d_(d), s_(s),
callback_(callback),
timer_(0.0f),
isEnd_(false),
loopType_(UpdateType::Common),
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
	updateFunc_(deltaTime);
}

bool TweenObject::IsEnd() const
{
	return isEnd_;
}

void TweenObject::End()
{
	isEnd_ = true;
}

float * TweenObject::GetValuePointer() const
{
	return value_;
}

void TweenObject::SetFunction(const std::function<float(float, float, float, float, float)> func)
{
	func_ = func;
}

void TweenObject::SetLoopType(const UpdateType type)
{
	if (type == UpdateType::Common)
		updateFunc_ = std::function<void(float)>([=](float deltaTime) { CommonUpdate(deltaTime); });
	else if (type == UpdateType::Loop)
		updateFunc_ = std::function<void(float)>([=](float deltaTime) { LoopUpdate(deltaTime); });
	else if (type == UpdateType::PingPong)
		updateFunc_ = std::function<void(float)>([=](float deltaTime) { PingPongUpdate(deltaTime); });
}

void TweenObject::CommonUpdate(float deltaTime)
{
	timer_ += deltaTime;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	if (value_ != nullptr)
		*value_ = func_(timer_, b_, c_, d_, s_);

	if (timer_ >= d_)
	{
		isEnd_ = true;
		Callback();
	}
}

void TweenObject::LoopUpdate(float deltaTime)
{
	timer_ += deltaTime;
	timer_ = MathHelper::Clamp(timer_, 0.0f, d_);

	if (value_ != nullptr)
		*value_ = func_(timer_, b_, c_, d_, s_);

	if (timer_ >= d_)
	{
		// 初期値と移動量を反転
		auto b = b_ + c_;
		c_ *= -1.0f;
		b_ = b;

		timer_ = 0.0f;
		sine_ *= -1.0f;
		 // 行きフラグ | sine_ < 0 < sine_  | 帰りフラグ
		if (sine_ > 0.0f)
			Callback();
	}
}

void TweenObject::PingPongUpdate(float deltaTime)
{
	timer_ += deltaTime * sine_;
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
		Callback();
	}
}

void TweenObject::Callback()
{
	if (callback_ != nullptr)
	{
		callback_();
		callback_ = nullptr;
	}
}
