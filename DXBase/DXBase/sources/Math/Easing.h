#pragma once

#include <cmath>

inline float EasingInBack(float t, float s = 1.70158f)
{
	return t * t * ((s + 1) * t - s);
}

inline float EasingInBounce(float t)
{
	t = 1.0f - t;
	if (t < 1 / 2.75f) {
		return 1.0f - (7.5625f * t * t);
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.75f);
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.9375f);
	}
	t -= 2.625f / 2.75f;
	return 1.0f - (7.5625f * t * t + 0.984375f);
}

inline float EasingInCirc(float t)
{
	return -(std::sqrt(1.0f - t * t) - 1.0f);
}

inline float EasingInCubic(float t)
{
	return t * t * t;
}

inline float EasingInElastic(float t, float s = 1.70158f)
{
	float p = 0.3f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	}
	else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	t -= 1.0f;
	return -(a * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p));
}

inline float EasingInExpo(float t)
{
	return (t == 0) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
}

inline float EasingInQuad(float t)
{
	return t * t;
}

inline float EasingInQuart(float t)
{
	return t * t * t * t;
}

inline float EasingInQuint(float t)
{
	return t * t * t * t * t;
}

inline float EasingInSine(float t)
{
	return -std::cos(t * (3.14159265359f / 2.0f)) + 1.0f;
}

inline float EasingOutBack(float t, float s = 1.70158f)
{
	t -= 1.0f;
	return t * t * ((s + 1.0f) * t + s) + 1.0f;
}

inline float EasingOutBounce(float t)
{
	if (t < 1.0f / 2.75f) {
		return 7.5625f * t * t;
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	t -= 2.625f / 2.75f;
	return static_cast<float>(7.5625 * t * t + 0.984375);
}

inline float EasingOutCirc(float t)
{
	t -= 1.0f;
	return std::sqrt(1.0f - t * t);
}

inline float EasingOutCubic(float t)
{
	t -= 1.0f;
	return t * t * t + 1.0f;
}

inline float EasingOutElastic(float t, float s = 1.70158f)
{
	float p = 0.3f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	}
	else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	return a * std::pow(2.0f, -10.0f * t) * sin((t - s) * (2.0f * 3.14159265359f) / p) + 1.0f;
}

inline float EasingOutExpo(float t)
{
	return (t == 1.0f) ? 1.0f : (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

inline float EasingOutQuad(float t)
{
	return -t * (t - 2.0f);
}

inline float EasingOutQuart(float t)
{
	t -= 1.0f;
	return static_cast<float>(1.0 - t * t * t * t);
}

inline float EasingOutQuint(float t)
{
	t -= 1.0f;
	return t * t * t * t * t + 1.0f;
}

inline float EasingOutSine(float t)
{
	return std::sin(t * (3.14159265359f / 2.0f));
}

inline float EasingInOutBack(float t, float s = 1.70158f)
{
	float k = 1.525f;
	t *= 2.0f;
	s *= k;
	if (t < 1) {
		return static_cast<float>(0.5 * (t * t * ((s + 1) * t - s)));
	}
	t -= 2;
	return static_cast<float>(0.5 * (t * t * ((s + 1) * t + s) + 2));
}

inline float EasingInOutBounce(float t)
{
	return (t < 0.5f)
		? (EasingInBounce(t * 2.0f) * 0.5f)
		: (EasingOutBounce(t * 2.0f - 1.0f) * 0.5f + 0.5f);
}

inline float EasingInOutCirc(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
	}
	t -= 2.0f;
	return 0.5f * (sqrt(1 - t * t) + 1);
}

inline float EasingInOutCubic(float t)
{
	t *= 2.0f;
	if (t < 1) {
		return static_cast<float>(0.5 * t * t * t);
	}
	t -= 2.0f;
	return static_cast<float>(0.5f * (t * t * t + 2.0f));
}

inline float EasingInOutElastic(float t, float s = 1.70158f)
{
	float p = 0.3f * 1.5f;
	float a = 1.0f;
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	if (a < 1.0f) {
		a = 1.0f;
		s = p / 4.0f;
	}
	else {
		s = p / (2.0f * 3.14159265359f) * std::asin(1.0f / a);
	}
	if (t < 1.0f) {
		t -= 1.0f;
		return -0.5f * (a * std::pow(2.0f, 10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p));
	}
	t -= 1.0f;
	return a * std::pow(2.0f, -10.0f * t) * std::sin((t - s) * (2.0f * 3.14159265359f) / p) * 0.5f + 1.0f;
}

inline float EasingInOutExpo(float t)
{
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
	}
	t -= 1.0f;
	return 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
}

inline float EasingInOutQuad(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t;
	}
	t -= 1.0f;
	return -0.5f * (t * (t - 2.0f) - 1.0f);
}

inline float EasingInOutQuart(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t;
	}
	t -= 2.0f;
	return -0.5f * (t * t * t * t - 2.0f);
}

inline float EasingInOutQuint(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t * t;
	}
	t -= 2.0f;
	return 0.5f * (t * t * t * t * t + 2.0f);
}

inline float EasingInOutSine(float t)
{
	return -0.5f * (std::cos(3.14159265359f * t) - 1.0f);
}

inline float EasingSinCurve(float t){
	return (t >= 1.0f) ? 0.0f : MathHelper::Sin(t * 180);
}

