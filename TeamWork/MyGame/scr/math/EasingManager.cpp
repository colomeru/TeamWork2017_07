#include "EasingManager.h"
#include "Easing.h"
#include "MathHelper.h"


EasingManager::EasingManager()
{
	easeList_.clear();
}

EasingManager::~EasingManager()
{

}

void EasingManager::Initialize()
{
}

void EasingManager::Update(float deltaTime)
{
	for (auto& ease : easeList_)
		ease->Update(deltaTime);
}

void EasingManager::Add(EasePtr ease)
{
	easeList_.push_back(ease);
}

void EasingManager::Clear()
{
	easeList_.clear();
}

void EasingManager::Remove()
{
	easeList_.remove_if([](EasePtr ease)
	{
		return ease->IsEnd();
	});
}

EaseNode::EaseNode(float* value, EaseType type, float b, float c, float d, std::function<void()> callback):
value_(value),
b_(b), c_(c), d_(d),
callback_(callback),
timer_(0.0f),
isEnd_(false)
{
}

EaseNode::~EaseNode()
{
	value_ = nullptr;
	callback_ = nullptr;
}

void EaseNode::Update(float deltaTime)
{
	timer_ += deltaTime;
	timer_ = MathHelper::Min(timer_, d_);
	*value_ = Easing::Linear(timer_, b_, c_, d_);

	if (timer_ >= d_)
	{
		if (callback_ != nullptr)
		{
			callback_();
			callback_ = nullptr;
		}
		isEnd_ = true;
	}
}

bool EaseNode::IsEnd() const
{
	return isEnd_;
}
