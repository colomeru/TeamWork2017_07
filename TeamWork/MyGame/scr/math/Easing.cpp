#include "Easing.h"
#include "MathHelper.h"
#include "Vector3.h"

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
	return c*(t*t*t + 1.0f) + b;
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
	return -c * MathHelper::Cos(t / d * (MathHelper::ToDegrees(MathHelper::Pi / 2.0f))) + c + b;
}

float Easing::EaseOutSine(float t, float b, float c, float d)
{
	return c * MathHelper::Sin(t / d * (MathHelper::ToDegrees(MathHelper::Pi / 2.0f))) + b;
}

float Easing::EaseInOutSine(float t, float b, float c, float d)
{
	return -c / 2.0f * (MathHelper::Cos(MathHelper::ToDegrees(MathHelper::Pi * t / d)) - 1.0f) + b;
}

float Easing::EaseInExpo(float t, float b, float c, float d)
{
	return c * MathHelper::Pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
}

float Easing::EaseOutExpo(float t, float b, float c, float d)
{
	return c * (-MathHelper::Pow(2.0f, -10.0f * t / d) + 1.0f) + b;
}

float Easing::EaseInOutExpo(float t, float b, float c, float d)
{
	t /= d / 2.0f;
	if (t < 1.0f) return c / 2.0f * MathHelper::Pow(2.0f, 10.0f * (t - 1.0f)) + b;
	t--;
	return c / 2.0f * (-MathHelper::Pow(2.0f, -10.0f * t) + 2.0f) + b;
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

float Easing::EaseInElastic(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float a = c;

	if (t <= 0.0f)
		return b;

	t /= d;
	if (t >= 1.0f)
		return b + c;

	float p = d * 0.3f;
	if (a < MathHelper::Abs(c))
	{
		a = c;
		s = p / 4.0f;
	}
	else
	{
		s = p / (2.0f * MathHelper::Pi) * MathHelper::ToRadians(MathHelper::ASin(c / a));
	}

	t--;
	float deg = MathHelper::ToDegrees((t * d - s) * (2.0f * MathHelper::Pi) / p);
	return (-(a * MathHelper::Pow(2.0f, 10.0f * t) * MathHelper::Sin(deg)) + b);
}

float Easing::EaseOutElastic(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float a = c;

	if (t <= 0.0f)
		return b;

	t /= d;
	if (t >= 1.0f)
		return b + c;

	float p = d * 0.3f;
	if (a < MathHelper::Abs(c))
	{
		a = c;
		s = p / 4.0f;
	}
	else
	{
		s = p / (MathHelper::Pi * 2.0f) * MathHelper::ToRadians(MathHelper::ASin(c / a));
	}

	float deg = MathHelper::ToDegrees(t * d - s) * (MathHelper::Pi * 2.0f) / p;
	return (a * MathHelper::Pow(2.0f, -10.0f * t) * MathHelper::Sin(deg) + c + b);
}

float Easing::EaseInOutElastic(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float a = c;

	if (t <= 0.0f)
		return b;

	t /= d / 2.0f;
	if (t >= 2.0f)
		return b + c;

	float p = d * 0.3f * 1.5f;
	if (a < MathHelper::Abs(c))
	{
		a = c;
		s = p / 4.0f;
	}
	else
	{
		s = p / (MathHelper::Pi * 2.0f) * MathHelper::ToRadians(MathHelper::ASin(c / a));
	}

	if (t < 1.0f)
	{
		t--;
		float deg = MathHelper::ToDegrees(t * d - s) * (MathHelper::Pi * 2.0f) / p;
		return -0.5f * (a * MathHelper::Pow(2.0f, 10.0f * t) * MathHelper::Sin(deg) + b);
	}
	else
	{
		t--;
		float deg = MathHelper::ToDegrees(t * d - s) * (MathHelper::Pi * 2.0f) / p;
		return (a * MathHelper::Pow(2.0f, -10.0f * t) * MathHelper::Sin(deg) * 0.5f + c + b);
	}
}

float Easing::EaseInBack(float t, float b, float c, float d, float s)
{
	t /= d;
	return c * t*t*((s + 1.0f)*t - s) + b;
}

float Easing::EaseOutBack(float t, float b, float c, float d, float s)
{
	t = t / d - 1.0f;
	return c * (t*t*((s + 1.0f) * t + s) + 1.0f) + b;
}

float Easing::EaseInOutBack(float t, float b, float c, float d, float s)
{
	t /= d / 2.0f;
	s *= 1.525f;
	if (t < 1.0f)	
		return c / 2.0f * (t*t*((s + 1.0f)*t - s)) + b;
	t -= 2.0f;
	return c / 2.0f * (t*t*((s + 1.0f) * t + s) + 2.0f) + b;
}

float Easing::EaseInBounce(float t, float b, float c, float d)
{
	return (b + c) - EaseOutBounce(d - t, 0.0f, c, d);
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

float Easing::EaseInOutBounce(float t, float b, float c, float d)
{
	if (t < d / 2.0f)
		return EaseInBounce(t * 2.0f, b, b + c, d) * 0.5f + b;
	else
		return EaseOutBounce(t * 2.0f - d, 0.0f, c, d) * 0.5f + b + c * 0.5f;
}

float Easing::LinearFT(float time, float endTime, float from, float to)
{
	return Linear(time, from, to - from, endTime);
}

float Easing::EaseInQuadFT(float time, float endTime, float from, float to)
{
	return EaseInQuad(time, from, to - from, endTime);
}

float Easing::EaseOutQuadFT(float time, float endTime, float from, float to)
{
	return EaseOutQuad(time, from, to - from, endTime);
}

float Easing::EaseInOutQuadFT(float time, float endTime, float from, float to)
{
	return EaseInOutQuad(time, from, to - from, endTime);
}

float Easing::EaseInCubicFT(float time, float endTime, float from, float to)
{
	return EaseInCubic(time, from, to - from, endTime);
}

float Easing::EaseOutCubicFT(float time, float endTime, float from, float to)
{
	return EaseOutCubic(time, from, to - from, endTime);
}

float Easing::EaseInOutCubicFT(float time, float endTime, float from, float to)
{
	return EaseInOutCubic(time, from, to - from, endTime);
}

float Easing::EaseInQuartFT(float time, float endTime, float from, float to)
{
	return EaseInQuart(time, from, to - from, endTime);
}

float Easing::EaseOutQuartFT(float time, float endTime, float from, float to)
{
	return EaseOutQuart(time, from, to - from, endTime);
}

float Easing::EaseInOutQuartFT(float time, float endTime, float from, float to)
{
	return EaseInOutQuart(time, from, to - from, endTime);
}

float Easing::EaseInQuintFT(float time, float endTime, float from, float to)
{
	return EaseInQuint(time, from, to - from, endTime);
}

float Easing::EaseOutQuintFT(float time, float endTime, float from, float to)
{
	return EaseOutQuint(time, from, to - from, endTime);
}

float Easing::EaseInOutQuintFT(float time, float endTime, float from, float to)
{
	return EaseInOutQuint(time, from, to - from, endTime);
}

float Easing::EaseInSineFT(float time, float endTime, float from, float to)
{
	return EaseInSine(time, from, to - from, endTime);
}

float Easing::EaseOutSineFT(float time, float endTime, float from, float to)
{
	return EaseOutSine(time, from, to - from, endTime);
}

float Easing::EaseInOutSineFT(float time, float endTime, float from, float to)
{
	return EaseInOutSine(time, from, to - from, endTime);
}

float Easing::EaseInExpoFT(float time, float endTime, float from, float to)
{
	return EaseInExpo(time, from, to - from, endTime);
}

float Easing::EaseOutExpoFT(float time, float endTime, float from, float to)
{
	return EaseOutExpo(time, from, to - from, endTime);
}

float Easing::EaseInOutExpoFT(float time, float endTime, float from, float to)
{
	return EaseInOutExpo(time, from, to - from, endTime);
}

float Easing::EaseInCircFT(float time, float endTime, float from, float to)
{
	return EaseInCirc(time, from, to - from, endTime);
}

float Easing::EaseOutCircFT(float time, float endTime, float from, float to)
{
	return EaseOutCirc(time, from, to - from, endTime);
}

float Easing::EaseInOutCircFT(float time, float endTime, float from, float to)
{
	return EaseInOutCirc(time, from, to - from, endTime);
}

float Easing::EaseInElasticFT(float time, float endTime, float from, float to)
{
	return EaseInElastic(time, from, to - from, endTime);
}

float Easing::EaseOutElasticFT(float time, float endTime, float from, float to)
{
	return EaseOutElastic(time, from, to - from, endTime);
}

float Easing::EaseInOutElasticFT(float time, float endTime, float from, float to)
{
	return EaseInOutElastic(time, from, to - from, endTime);
}

float Easing::EaseInBackFT(float time, float endTime, float from, float to, float s)
{
	return EaseInBack(time, from, to - from, endTime, s);
}

float Easing::EaseOutBackFT(float time, float endTime, float from, float to, float s)
{
	return EaseOutBack(time, from, to - from, endTime, s);
}

float Easing::EaseInOutBackFT(float time, float endTime, float from, float to, float s)
{
	return EaseInOutBack(time, from, to - from, endTime, s);
}

float Easing::EaseOutBounceFT(float time, float endTime, float from, float to)
{
	return EaseOutBounce(time, from, to - from, endTime);
}

float Easing::EaseInBounceFT(float time, float endTime, float from, float to)
{
	return EaseInBounce(time, from, to - from, endTime);
}

float Easing::EaseInOutBounceFT(float time, float endTime, float from, float to)
{
	return EaseInOutBounce(time, from, to - from, endTime);
}

Vector3 Easing::Linear(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = Linear(t, b.x, c.x, d);
	result.y = Linear(t, b.y, c.y, d);
	result.z = Linear(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInQuad(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInQuad(t, b.x, c.x, d);
	result.y = EaseInQuad(t, b.y, c.y, d);
	result.z = EaseInQuad(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutQuad(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutQuad(t, b.x, c.x, d);
	result.y = EaseOutQuad(t, b.y, c.y, d);
	result.z = EaseOutQuad(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutQuad(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutQuad(t, b.x, c.x, d);
	result.y = EaseInOutQuad(t, b.y, c.y, d);
	result.z = EaseInOutQuad(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInCubic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInCubic(t, b.x, c.x, d);
	result.y = EaseInCubic(t, b.y, c.y, d);
	result.z = EaseInCubic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutCubic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutCubic(t, b.x, c.x, d);
	result.y = EaseOutCubic(t, b.y, c.y, d);
	result.z = EaseOutCubic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutCubic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutCubic(t, b.x, c.x, d);
	result.y = EaseInOutCubic(t, b.y, c.y, d);
	result.z = EaseInOutCubic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInQuart(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInQuart(t, b.x, c.x, d);
	result.y = EaseInQuart(t, b.y, c.y, d);
	result.z = EaseInQuart(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutQuart(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutQuart(t, b.x, c.x, d);
	result.y = EaseOutQuart(t, b.y, c.y, d);
	result.z = EaseOutQuart(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutQuart(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutQuart(t, b.x, c.x, d);
	result.y = EaseInOutQuart(t, b.y, c.y, d);
	result.z = EaseInOutQuart(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInQuint(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInQuint(t, b.x, c.x, d);
	result.y = EaseInQuint(t, b.y, c.y, d);
	result.z = EaseInQuint(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutQuint(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutQuint(t, b.x, c.x, d);
	result.y = EaseOutQuint(t, b.y, c.y, d);
	result.z = EaseOutQuint(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutQuint(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutQuint(t, b.x, c.x, d);
	result.y = EaseInOutQuint(t, b.y, c.y, d);
	result.z = EaseInOutQuint(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInSine(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInSine(t, b.x, c.x, d);
	result.y = EaseInSine(t, b.y, c.y, d);
	result.z = EaseInSine(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutSine(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutSine(t, b.x, c.x, d);
	result.y = EaseOutSine(t, b.y, c.y, d);
	result.z = EaseOutSine(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutSine(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutSine(t, b.x, c.x, d);
	result.y = EaseInOutSine(t, b.y, c.y, d);
	result.z = EaseInOutSine(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInExpo(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInExpo(t, b.x, c.x, d);
	result.y = EaseInExpo(t, b.y, c.y, d);
	result.z = EaseInExpo(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutExpo(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutExpo(t, b.x, c.x, d);
	result.y = EaseOutExpo(t, b.y, c.y, d);
	result.z = EaseOutExpo(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutExpo(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutExpo(t, b.x, c.x, d);
	result.y = EaseInOutExpo(t, b.y, c.y, d);
	result.z = EaseInOutExpo(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInCirc(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInCirc(t, b.x, c.x, d);
	result.y = EaseInCirc(t, b.y, c.y, d);
	result.z = EaseInCirc(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutCirc(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutCirc(t, b.x, c.x, d);
	result.y = EaseOutCirc(t, b.y, c.y, d);
	result.z = EaseOutCirc(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutCirc(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutCirc(t, b.x, c.x, d);
	result.y = EaseInOutCirc(t, b.y, c.y, d);
	result.z = EaseInOutCirc(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInElastic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInElastic(t, b.x, c.x, d);
	result.y = EaseInElastic(t, b.y, c.y, d);
	result.z = EaseInElastic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseOutElastic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutElastic(t, b.x, c.x, d);
	result.y = EaseOutElastic(t, b.y, c.y, d);
	result.z = EaseOutElastic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutElastic(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutElastic(t, b.x, c.x, d);
	result.y = EaseInOutElastic(t, b.y, c.y, d);
	result.z = EaseInOutElastic(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInBack(float t, const Vector3& b, const Vector3& c, float d, float s)
{
	Vector3 result;
	result.x = EaseInBack(t, b.x, c.x, d, s);
	result.y = EaseInBack(t, b.y, c.y, d, s);
	result.z = EaseInBack(t, b.z, c.z, d, s);
	return result;
}

Vector3 Easing::EaseOutBack(float t, const Vector3& b, const Vector3& c, float d, float s)
{
	Vector3 result;
	result.x = EaseOutBack(t, b.x, c.x, d, s);
	result.y = EaseOutBack(t, b.y, c.y, d, s);
	result.z = EaseOutBack(t, b.z, c.z, d, s);
	return result;
}

Vector3 Easing::EaseInOutBack(float t, const Vector3& b, const Vector3& c, float d, float s)
{
	Vector3 result;
	result.x = EaseInOutBack(t, b.x, c.x, d, s);
	result.y = EaseInOutBack(t, b.y, c.y, d, s);
	result.z = EaseInOutBack(t, b.z, c.z, d, s);
	return result;
}

Vector3 Easing::EaseOutBounce(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseOutBounce(t, b.x, c.x, d);
	result.y = EaseOutBounce(t, b.y, c.y, d);
	result.z = EaseOutBounce(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInBounce(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInBounce(t, b.x, c.x, d);
	result.y = EaseInBounce(t, b.y, c.y, d);
	result.z = EaseInBounce(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::EaseInOutBounce(float t, const Vector3& b, const Vector3& c, float d)
{
	Vector3 result;
	result.x = EaseInOutBounce(t, b.x, c.x, d);
	result.y = EaseInOutBounce(t, b.y, c.y, d);
	result.z = EaseInOutBounce(t, b.z, c.z, d);
	return result;
}

Vector3 Easing::LinearFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = LinearFT(time, endTime, from.x, to.x);
	result.y = LinearFT(time, endTime, from.y, to.y);
	result.z = LinearFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInQuadFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInQuadFT(time, endTime, from.x, to.x);
	result.y = EaseInQuadFT(time, endTime, from.y, to.y);
	result.z = EaseInQuadFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutQuadFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutQuadFT(time, endTime, from.x, to.x);
	result.y = EaseOutQuadFT(time, endTime, from.y, to.y);
	result.z = EaseOutQuadFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutQuadFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutQuadFT(time, endTime, from.x, to.x);
	result.y = EaseInOutQuadFT(time, endTime, from.y, to.y);
	result.z = EaseInOutQuadFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInCubicFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInCubicFT(time, endTime, from.x, to.x);
	result.y = EaseInCubicFT(time, endTime, from.y, to.y);
	result.z = EaseInCubicFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutCubicFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutCubicFT(time, endTime, from.x, to.x);
	result.y = EaseOutCubicFT(time, endTime, from.y, to.y);
	result.z = EaseOutCubicFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutCubicFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutCubicFT(time, endTime, from.x, to.x);
	result.y = EaseInOutCubicFT(time, endTime, from.y, to.y);
	result.z = EaseInOutCubicFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInQuartFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInQuartFT(time, endTime, from.x, to.x);
	result.y = EaseInQuartFT(time, endTime, from.y, to.y);
	result.z = EaseInQuartFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutQuartFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutQuartFT(time, endTime, from.x, to.x);
	result.y = EaseOutQuartFT(time, endTime, from.y, to.y);
	result.z = EaseOutQuartFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutQuartFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutQuartFT(time, endTime, from.x, to.x);
	result.y = EaseInOutQuartFT(time, endTime, from.y, to.y);
	result.z = EaseInOutQuartFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInQuintFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInQuintFT(time, endTime, from.x, to.x);
	result.y = EaseInQuintFT(time, endTime, from.y, to.y);
	result.z = EaseInQuintFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutQuintFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutQuintFT(time, endTime, from.x, to.x);
	result.y = EaseOutQuintFT(time, endTime, from.y, to.y);
	result.z = EaseOutQuintFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutQuintFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutQuintFT(time, endTime, from.x, to.x);
	result.y = EaseInOutQuintFT(time, endTime, from.y, to.y);
	result.z = EaseInOutQuintFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInSineFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInSineFT(time, endTime, from.x, to.x);
	result.y = EaseInSineFT(time, endTime, from.y, to.y);
	result.z = EaseInSineFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutSineFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutSineFT(time, endTime, from.x, to.x);
	result.y = EaseOutSineFT(time, endTime, from.y, to.y);
	result.z = EaseOutSineFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutSineFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutSineFT(time, endTime, from.x, to.x);
	result.y = EaseInOutSineFT(time, endTime, from.y, to.y);
	result.z = EaseInOutSineFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInExpoFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInExpoFT(time, endTime, from.x, to.x);
	result.y = EaseInExpoFT(time, endTime, from.y, to.y);
	result.z = EaseInExpoFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutExpoFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutExpoFT(time, endTime, from.x, to.x);
	result.y = EaseOutExpoFT(time, endTime, from.y, to.y);
	result.z = EaseOutExpoFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutExpoFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutExpoFT(time, endTime, from.x, to.x);
	result.y = EaseInOutExpoFT(time, endTime, from.y, to.y);
	result.z = EaseInOutExpoFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInCircFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInCircFT(time, endTime, from.x, to.x);
	result.y = EaseInCircFT(time, endTime, from.y, to.y);
	result.z = EaseInCircFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutCircFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutCircFT(time, endTime, from.x, to.x);
	result.y = EaseOutCircFT(time, endTime, from.y, to.y);
	result.z = EaseOutCircFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutCircFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutCircFT(time, endTime, from.x, to.x);
	result.y = EaseInOutCircFT(time, endTime, from.y, to.y);
	result.z = EaseInOutCircFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInElasticFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInElasticFT(time, endTime, from.x, to.x);
	result.y = EaseInElasticFT(time, endTime, from.y, to.y);
	result.z = EaseInElasticFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutElasticFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutElasticFT(time, endTime, from.x, to.x);
	result.y = EaseOutElasticFT(time, endTime, from.y, to.y);
	result.z = EaseOutElasticFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutElasticFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutElasticFT(time, endTime, from.x, to.x);
	result.y = EaseInOutElasticFT(time, endTime, from.y, to.y);
	result.z = EaseInOutElasticFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInBackFT(float time, float endTime, const Vector3 & from, const Vector3 & to, float s)
{
	Vector3 result;
	result.x = EaseInBackFT(time, endTime, from.x, to.x, s);
	result.y = EaseInBackFT(time, endTime, from.y, to.y, s);
	result.z = EaseInBackFT(time, endTime, from.z, to.z, s);
	return result;
}

Vector3 Easing::EaseOutBackFT(float time, float endTime, const Vector3 & from, const Vector3 & to, float s)
{
	Vector3 result;
	result.x = EaseOutBackFT(time, endTime, from.x, to.x, s);
	result.y = EaseOutBackFT(time, endTime, from.y, to.y, s);
	result.z = EaseOutBackFT(time, endTime, from.z, to.z, s);
	return result;
}

Vector3 Easing::EaseInOutBackFT(float time, float endTime, const Vector3 & from, const Vector3 & to, float s)
{
	Vector3 result;
	result.x = EaseInOutBackFT(time, endTime, from.x, to.x, s);
	result.y = EaseInOutBackFT(time, endTime, from.y, to.y, s);
	result.z = EaseInOutBackFT(time, endTime, from.z, to.z, s);
	return result;
}

Vector3 Easing::EaseInBounceFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInBounceFT(time, endTime, from.x, to.x);
	result.y = EaseInBounceFT(time, endTime, from.y, to.y);
	result.z = EaseInBounceFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseOutBounceFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseOutBounceFT(time, endTime, from.x, to.x);
	result.y = EaseOutBounceFT(time, endTime, from.y, to.y);
	result.z = EaseOutBounceFT(time, endTime, from.z, to.z);
	return result;
}

Vector3 Easing::EaseInOutBounceFT(float time, float endTime, const Vector3 & from, const Vector3 & to)
{
	Vector3 result;
	result.x = EaseInOutBounceFT(time, endTime, from.x, to.x);
	result.y = EaseInOutBounceFT(time, endTime, from.y, to.y);
	result.z = EaseInOutBounceFT(time, endTime, from.z, to.z);
	return result;
}