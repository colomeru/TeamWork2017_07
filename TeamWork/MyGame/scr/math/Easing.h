#pragma once

//�C�[�W���O�֐�
class Easing
{
public:
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float Linear(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseOutBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInOutBack(float t, float b, float c, float d, float s);

	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseOutBounce(float t, float b, float c, float d);
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInBounce(float t, float b, float c, float d);
	// ������
	// t = ���� / b = �J�n�_ / c = �ړ��� / d = �I������
	static float EaseInOutBounce(float t, float b, float c, float d);
};