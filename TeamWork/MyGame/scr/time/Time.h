#pragma once
#include "FPS.h"

class Time
{
public:
	Time() = default;

	// 初期化
	void Initialize();
	// 更新処理
	void Update();
	
public:
	// 前フレームと今フレームの時間差
	static float DeltaTime;

private:
	// 前フレームの時間
	int prevTime;
};