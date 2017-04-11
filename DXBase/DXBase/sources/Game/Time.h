#pragma once

class Time {
public:
	Time();
	static Time &GetInstance();
	void update();
	float deltaTime();
	void draw_fps();
private:
	float m_LastFrame;
	float m_LastTime;
	float m_RealtimeSinceStartup;
	float m_FrameCount;
	float m_DeltaTime;
private:
	int counter = 0;
	int FpsTime[2] = { 0, };
	int FpsTime_i; 
	double Fps = 0.0;
};