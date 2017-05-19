#pragma once

//イージング関数
class Easing
{
public:
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float Linear(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInQuad(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutQuad(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutQuad(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInCubic(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutCubic(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutCubic(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInQuart(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutQuart(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutQuart(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInQuint(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutQuint(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutQuint(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInSine(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutSine(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutSine(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInExpo(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutExpo(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutExpo(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInCirc(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutCirc(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutCirc(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間 / s > 1.0f
	static float EaseInBack(float t, float b, float c, float d, float s);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間 / s > 1.0f
	static float EaseOutBack(float t, float b, float c, float d, float s);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間 / s > 1.0f
	static float EaseInOutBack(float t, float b, float c, float d, float s);

	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseOutBounce(float t, float b, float c, float d);
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInBounce(float t, float b, float c, float d);
	// 未完成
	// t = 時間 / b = 開始点 / c = 移動量 / d = 終了時間
	static float EaseInOutBounce(float t, float b, float c, float d);
};