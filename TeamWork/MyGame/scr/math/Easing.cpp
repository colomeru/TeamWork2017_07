#include "Easing.h"
#include "MathHelper.h"
#include <cmath>

float Easing::Linear(float t, float b, float c, float d)
{
	return c * t / d + b;
}

float Easing::EaseInQuad(float t, float b, float c, float d)
{
	t /= d;
	return c*t*t + b;	
}

float Easing::EaseOutQuad(float t, float b, float c, float d)
{
	t /= d;
	return -c * t*(t - 2.0f) + b;
}

float Easing::EaseInOutQuad(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return c / 2.0f * t*t + b;
	t--;
	return -c / 2.0f * (t*(t - 2.0f) - 1.0f) + b;
}

float Easing::EaseInCubic(float t, float b, float c, float d)
{
	t /= d;
	return c*t*t*t + b;
}

float Easing::EaseOutCubic(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}

float Easing::EaseInOutCubic(float t, float b, float c, float d)
{
	t /= d / 2.0;
	if (t < 1.0f) return c / 2.0f*t*t*t + b;
	t -= 2.0f;
	return c / 2.0f * (t*t*t + 2.0f) + b;
}

float Easing::EaseInQuart(float t, float b, float c, float d)
{
	t /= d;
	return c*t*t*t*t + b;
}

float Easing::EaseOutQuart(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return -c * (t*t*t*t - 1.0f) + b;
}

float Easing::EaseInOutQuart(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return c / 2.0f * t*t*t*t + b;
	t -= 2.0f;
	return -c / 2.0f * (t*t*t*t - 2.0f) + b;
}

float Easing::EaseInQuint(float t, float b, float c, float d)
{
	t /= d;
	return c*t*t*t*t*t + b;
}

float Easing::EaseOutQuint(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1.0f) + b;
}

float Easing::EaseInOutQuint(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return c / 2.0f*t*t*t*t*t + b;
	t -= 2.0f;
	return c / 2.0f*(t*t*t*t*t + 2.0f) + b;
}

float Easing::EaseInSine(float t, float b, float c, float d)
{
	return -c * MathHelper::Cos(t / d * (MathHelper::Pi / 2.0f)) + c + b;
}

float Easing::EaseOutSine(float t, float b, float c, float d)
{
	return c * MathHelper::Sin(t / d * (MathHelper::Pi / 2.0f)) + b;
}

float Easing::EaseInOutSine(float t, float b, float c, float d)
{
	return -c / 2.0f * (MathHelper::Cos(MathHelper::Pi * t / d) - 1.0f) + b;
}

float Easing::EaseInExpo(float t, float b, float c, float d)
{
	return c * powf(2.0f, 10.0f * (t / d - 1)) + b;
}

float Easing::EaseOutExpo(float t, float b, float c, float d)
{
	return c * (-powf(2.0f, -10.0f * t / d) + 1.0f) + b;
}

float Easing::EaseInOutExpo(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return c / 2.0f * powf(2.0f, 10.0f * (t - 1.0f)) + b;
	t--;
	return c / 2.0f * (-powf(2.0f, -10.0f * t) + 2.0f) + b;
}

float Easing::EaseInCirc(float t, float b, float c, float d)
{
	t /= d;
	return -c * (MathHelper::Sqrt(1.0f - t*t) - 1.0f) + b;
}

float Easing::EaseOutCirc(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return c * MathHelper::Sqrt(1.0f - t*t) + b;
}

float Easing::EaseInOutCirc(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return -c / 2.0f * (MathHelper::Sqrt(1.0f - t*t) - 1.0f) + b;
	t -= 2.0f;
	return c / 2.0f * (MathHelper::Sqrt(1.0f - t*t) + 1.0f) + b;
}



float Easing::EaseInBack(float t, float b, float c, float d, float s)
{
	t /= d;
	return c * t*t*((s + 1.0f)*t - s) + b;
}

float Easing::EaseOutBack(float t, float b, float c, float d, float s)
{
	t = t / d - 1.0f;
	auto temp = c * (t*t*((s + 1.0f) * t*s) + 1.0f) + b;
	return c * (t*t*((s + 1.0f) * t + s) + 1.0f) + b;
}

float Easing::EaseInOutBack(float t, float b, float c, float d, float s)
{
	s *= 1.525f;
	if (t / 2.0f < 1.0f)	return c * (t*t*((s + 1.0f)*t)) + b;
	t -= 2.0f;
	return c / 2.0f * (t*t*((s + 1.0f)*t + s) + 2.0f) + b;
}

float Easing::EaseOutBounce(float t, float b, float c, float d)
{
	t /= d;

	if (t < 1.0f / 2.75f)
		return c*(7.5625f*t*t) + b;
	else if (t < 2.0f / 2.75f)
	{
		t -= 1.5f / 2.75f;
		return c*(7.5625f*t*t + 0.75f) + b;
	}
	else if (t< 2.5f / 2.75f)
	{
		t -= 2.25f / 2.75f;
		return c*(7.5625f*t*t + 0.9375f) + b;
	}
	else
	{
		t -= 2.625f / 2.75f;
		return c*(7.5625f*t*t + 0.984375f) + b;
	}
}

float Easing::EaseInBounce(float t, float b, float c, float d)
{
	return (b + c) - EaseOutBounce(d - t, 0.0f, c, d);
}

float Easing::EaseInOutBounce(float t, float b, float c, float d)
{
	if (t < d / 2.0f)
		return EaseInBounce(t * 2.0f, b + c, c, d) * 0.5f;
	else
		return EaseOutBounce(t * 2.0f - d, 0.0f, c, d) * 0.5f + c * 0.5f;
}

//float Easing::InBounce(float t, float totaltime, float max, float min)
//{
//	return max - OutBounce(totaltime - t, totaltime, max - min, 0) + min;
//}

//float Easing::InOutBounce(float t, float totaltime, float max, float min)
//{
//	if (t < totaltime / 2)
//		return InBounce(t * 2, totaltime, max - min, max)*0.5 + min;
//	else
//		return OutBounce(t * 2 - totaltime, totaltime, max - min, 0)*0.5 + min + (max - min)*0.5;
//}