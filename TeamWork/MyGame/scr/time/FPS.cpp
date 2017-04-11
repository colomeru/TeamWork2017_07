#include "FPS.h"
#include "DxLib.h"

float FPS::GetFPS;
// �T���v����
static const int N = 60;
// �ݒ�FPS
static const int SetFPS = 60;

// �R���X�g���N�^
FPS::FPS() :
startTime_(0),
count_(0),
fps_(0.0f)
{
}

void FPS::Update()
{
	if (count_ == 0)
		startTime_ = GetNowCount();

	if (count_ == N)
	{
		int t = GetNowCount();
		fps_ = 1000.0f / ((t - startTime_) / (float)N);
 		count_ = 0;
		startTime_ = t;
	}

	count_++;
	GetFPS = fps_;
}

void FPS::Wait()
{
	int tookTime = GetNowCount() - startTime_;			// ������������
	int waitTime = count_ * 1000 / SetFPS - tookTime;	// �҂ׂ�����
	if (waitTime > 0)
		Sleep(waitTime);
}
