#include "TweenManager.h"
#include "../math/MathHelper.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"

TweenManager::TweenManager() :
	loopCount_(0),
	updateType_(UpdateType::Common)
{
	tweenList_.clear();
	easeFuncMap_.clear();

	easeFuncMap_[EaseType::Linear] = std::bind(&Easing::LinearFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInQuad] = std::bind(&Easing::EaseInQuadFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutQuad] = std::bind(&Easing::EaseOutQuadFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutQuad] = std::bind(&Easing::EaseInOutQuadFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInCubic] = std::bind(&Easing::EaseInCubicFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutCubic] = std::bind(&Easing::EaseOutCubicFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutCubic] = std::bind(&Easing::EaseInOutCubicFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInQuart] = std::bind(&Easing::EaseInQuartFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutQuart] = std::bind(&Easing::EaseOutQuartFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutQuart] = std::bind(&Easing::EaseInOutQuartFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInQuint] = std::bind(&Easing::EaseInQuintFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutQuint] = std::bind(&Easing::EaseOutQuintFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutQuint] = std::bind(&Easing::EaseInOutQuintFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInSine] = std::bind(&Easing::EaseInSineFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutSine] = std::bind(&Easing::EaseOutSineFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutSine] = std::bind(&Easing::EaseInOutSineFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInExpo] = std::bind(&Easing::EaseInExpoFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutExpo] = std::bind(&Easing::EaseOutExpoFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutExpo] = std::bind(&Easing::EaseInOutExpoFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInCirc] = std::bind(&Easing::EaseInCircFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutCirc] = std::bind(&Easing::EaseOutCircFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutCirc] = std::bind(&Easing::EaseInOutCircFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInElastic] = std::bind(&Easing::EaseInElasticFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutElastic] = std::bind(&Easing::EaseOutElasticFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutElastic] = std::bind(&Easing::EaseInOutElasticFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInBack] = std::bind(&Easing::EaseInBackFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutBack] = std::bind(&Easing::EaseOutBackFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutBack] = std::bind(&Easing::EaseInOutBackFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInBounce] = std::bind(&Easing::EaseInBounceFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseOutBounce] = std::bind(&Easing::EaseOutBounceFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	easeFuncMap_[EaseType::EaseInOutBounce] = std::bind(&Easing::EaseInOutBounceFunc, ease_, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

TweenManager::~TweenManager()
{
	Clear();
}

void TweenManager::Initialize()
{
	Clear();
}

void TweenManager::Update(const float deltaTime)
{
	for (auto& ease : tweenList_)
		ease->Update(deltaTime);
}

void TweenManager::Remove()
{
	tweenList_.remove_if([](TweenPtr ease)
	{
		return ease->IsEnd();
	});
}

void TweenManager::Clear()
{
	tweenList_.clear();
}

void TweenManager::Play(float scale)
{
	for (auto& ease : tweenList_)
		ease->Play(scale);
}

void TweenManager::Stop(const float * value)
{
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() != value)
			continue;

		ease->Stop();
		return;
	}
}

void TweenManager::Stop(const Vector2 * value)
{
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() == &value->x ||
			ease->GetValuePointer() == &value->y)
		{
			ease->Stop();
		}
	}
}

void TweenManager::Stop(const Vector3 * value)
{
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() == &value->x ||
			ease->GetValuePointer() == &value->y ||
			ease->GetValuePointer() == &value->z)
		{
			ease->Stop();
		}
	}
}

void TweenManager::StopAll()
{
	for (auto& ease : tweenList_)
		ease->Stop();
}

void TweenManager::Delay(const float delay, const std::function<void()>& func, float * debug)
{
	Add(debug, Linear, 0.0f, 1.0f, delay, func);
}

void TweenManager::SetLoopCount(const int count)
{
	loopCount_ = count;
}

void TweenManager::SetLoopType(const UpdateType type)
{
	updateType_ = type;
}

void TweenManager::ResetOption()
{
	loopCount_ = 0;
	updateType_ = UpdateType::Common;
}

void TweenManager::Add(float* value, const EaseType& type, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	auto tweenPtr_ = std::make_shared<TweenObject>(value, b, c, d, callback, s);
	tweenPtr_->SetFunction(easeFuncMap_[type]);
	tweenPtr_->SetLoopType(updateType_);
	tweenPtr_->SetLoopCount(loopCount_);
	for (auto& e : tweenList_)
	{
		if (e->GetValuePointer() == value)
		{
			e = tweenPtr_;
			return;
		}
	}
	tweenList_.push_back(tweenPtr_);
}

void TweenManager::Add(Vector2 * value, const EaseType & type, const Vector2 & b, const Vector2 & c, const float d, const std::function<void()>& callback, const float s)
{
	Add(&value->x, type, b.x, c.x, d, callback, s);
	Add(&value->y, type, b.y, c.y, d, nullptr, s);
}

void TweenManager::Add(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3 & c, const float d, const std::function<void()>& callback, const float s)
{
	Add(&value->x, type, b.x, c.x, d, callback, s);
	Add(&value->y, type, b.y, c.y, d, nullptr, s);
	Add(&value->z, type, b.z, c.z, d, nullptr, s);
}

void TweenManager::Add(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Add(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Add(const EaseType & type, Vector3 * from, const Vector3& to, const float d, const std::function<void()>& callback, const float s)
{
	Add(from, type, *from, to - *from, d, callback, s);
}

void TweenManager::Loop(float * value, const EaseType & type, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

void TweenManager::Loop(Vector2 * value, const EaseType & type, const Vector2 & b, const Vector2 c, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

void TweenManager::Loop(Vector3 * value, const EaseType & type, const Vector3 & b, const Vector3 c, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

void TweenManager::Loop(const EaseType & type, float * from, const float to, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(from, type, *from, to - *from, d, callback, s);
	ResetOption();
}

void TweenManager::Loop(const EaseType & type, Vector2 * from, const Vector2 to, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(from, type, *from, to - *from, d, callback, s);
	ResetOption();
}

void TweenManager::Loop(const EaseType & type, Vector3 * from, const Vector3 & to, const float d, const std::function<void()>& callback, const float s)
{
	loopCount_ = 0;
	updateType_ = UpdateType::Loop;
	Add(from, type, *from, to - *from, d, callback, s);
	ResetOption();
}

void TweenManager::LoopOnce(float * value, const EaseType & type, const float b, const float c, const float d, std::function<void()> callback, const float s)
{
	loopCount_ = 1;
	updateType_ = UpdateType::Loop;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

void TweenManager::LoopCount(float * value, const EaseType & type, const int count, const float b, const float c, const float d, std::function<void()> callback, const float s)
{
	loopCount_ = count;
	updateType_ = UpdateType::Loop;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

void TweenManager::LoopPingPong(float * value, const EaseType & type, const float b, const float c, const float d, const std::function<void()>& callback, const float s)
{
	updateType_ = UpdateType::PingPong;
	Add(value, type, b, c, d, callback, s);
	ResetOption();
}

bool TweenManager::Cancel(const float * value)
{
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() != value)
			continue;

		ease->End();
		return true;
	}
	return false;
}

bool TweenManager::Cancel(const Vector2 * value)
{
	bool result = false;
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() == &value->x ||
			ease->GetValuePointer() == &value->y)
		{
			ease->End();
			result = true;
		}
	}
	return result;
}

bool TweenManager::Cancel(const Vector3 * value)
{
	bool result = false;
	for (auto& ease : tweenList_)
	{
		if (ease->GetValuePointer() == &value->x ||
			ease->GetValuePointer() == &value->y ||
			ease->GetValuePointer() == &value->z)
		{
			ease->End();
			result = true;
		}
	}
	return result;
}

bool TweenManager::DelayCancel(const float * value)
{
	return Cancel(value);
}