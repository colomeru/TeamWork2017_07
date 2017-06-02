#include "Time.h"
#include "../math/MathHelper.h"
#include <DxLib.h>

float Time::DeltaTime;

void Time::Initialize()
{
	prevTime = GetNowCount();
}

// XVˆ—
void Time::Update()
{
	DeltaTime = (float)((GetNowCount() - (MathHelper::Abs(prevTime))) / 1000.0f);
	prevTime = GetNowCount();
}