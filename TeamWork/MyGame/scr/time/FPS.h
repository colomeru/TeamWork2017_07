#pragma once

class FPS
{
public:
	// コンストラクタ
	FPS();

	// 更新処理
	void Update();
	// 待機
	void Wait();

public:
	// FPS
	static float GetFPS;

private:
	// 測定開始
	int		startTime_;
	// カウンタ
	int		count_;
	// fps
	float	fps_;

};