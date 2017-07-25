#include "MethodTimer.h"
#include"../math/MathHelper.h"

MethodTimer::MethodTimer() :count_{ 0 }
{
	Initialize();
}

MethodTimer::~MethodTimer()
{
	targetFuncList_.clear();
	emptyCount_.clear();
	MAX_EMPTY_COUNT.clear();
}

void MethodTimer::Initialize()
{
	//それぞれのリストを初期化し、値を初期化する
	MAX_EMPTY_COUNT.clear();
	emptyCount_.clear();
	NumReset();
	targetFuncList_.clear();
}

void MethodTimer::InnerInitialize()
{
	//初期化して
	Initialize();
	//Actionで行われる加算分を引く
	count_--;
}

void MethodTimer::NumReset()
{
	count_ = 0;
	for (int i = 0; i < (int)emptyCount_.size();i++) {
		emptyCount_[i] = MAX_EMPTY_COUNT[i];
	}
}

void MethodTimer::AddEmpty(int size)
{
	//カウントを追加して
	MAX_EMPTY_COUNT.push_back(size);
	emptyCount_.push_back(size);

	//何番目かを計算して
	int emptyCount = emptyCount_.size()-1;
	//識別用の空の関数を追加する
	targetFuncList_.push_back([this, emptyCount] {CountEmpty(emptyCount); });
}

void MethodTimer::Add(std::function<void()> func)
{
	targetFuncList_.push_back(func);
}

void MethodTimer::Action()
{
	if ((int)targetFuncList_.size() <= count_)return;

	targetFuncList_[count_]();
	count_++;
}

void MethodTimer::ToLastAction()
{
	if ((int)targetFuncList_.size() <= count_)return;

	count_ = targetFuncList_.size() - 1;

	targetFuncList_[count_]();
	count_++;

}

void MethodTimer::CountEmpty(int count)
{
	if ((int)targetFuncList_.size() <= count_)return;

	//自身のカウントを減らして
	emptyCount_[count]--;
	//カウントが終了してなかったら、カウント加算を帳消しにして関数を抜ける
	if (emptyCount_[count] > 0) {
		count_--;
		return;
	}

}
