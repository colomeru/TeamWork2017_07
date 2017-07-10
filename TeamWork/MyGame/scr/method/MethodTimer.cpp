#include "MethodTimer.h"
#include"../math/MathHelper.h"

MethodTimer::MethodTimer() :count_(0)
{
	Initialize();
}

void MethodTimer::Initialize()
{
	NumReset();
	targetFuncList_.clear();
}

void MethodTimer::NumReset()
{
	count_ = 0;
}

void MethodTimer::AddEmpty(int size)
{
	for (int i = 0; i < size; i++) {
		targetFuncList_.push_back([] {});
	}
}

void MethodTimer::Add(std::function<void()> func)
{
	targetFuncList_.push_back(func);
}

void MethodTimer::Action()
{
	if (targetFuncList_.size() <= count_)return;

	targetFuncList_[count_]();
	count_++;
}

void MethodTimer::ToLastAction()
{
	if (targetFuncList_.size() <= count_)return;

	count_ = targetFuncList_.size() - 1;

	targetFuncList_[count_]();
	count_++;

}
