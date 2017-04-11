#include "Time.h"

#include <DxLib.h>

Time::Time() :
	m_LastFrame(0), m_LastTime(0), m_RealtimeSinceStartup(0), m_FrameCount(0), m_DeltaTime(0) {
}

Time & Time::GetInstance() {
	static Time instance;
	return instance;
}

void Time::update(){
	m_FrameCount++;
	m_RealtimeSinceStartup = m_FrameCount / 60;

	float currentFrame = m_FrameCount;
	if (currentFrame != m_LastFrame) {
		float currentTime = m_RealtimeSinceStartup;
		m_DeltaTime = currentTime - m_LastTime;
		m_LastTime = currentTime;
		m_LastFrame = currentFrame;
	}
}

float Time::deltaTime(){
	return m_DeltaTime;
}

void Time::draw_fps(){
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1周目の時間取得
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50周目の時間取得
		Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//測定した値からfpsを計算
		FpsTime_i = 0;//カウントを初期化
	}
	else
		FpsTime_i++;//現在何周目かカウント
	//if (Fps != 0)
	//	DrawFormatString(1760, 960, GetColor(255, 255, 255), "FPS %.1f", Fps); //fpsを表示
}


