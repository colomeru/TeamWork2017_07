#pragma once

struct Vector3;

//�C�[�W���O�֐�
class Easing
{
public:
	/* �J�n�_���ړ��ʃp�^�[�� */
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float Linear(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuad(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutCubic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuart(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutQuint(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutSine(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutExpo(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutCirc(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutElastic(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseOutBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static float EaseInOutBack(float t, float b, float c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInBounce(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseOutBounce(float t, float b, float c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static float EaseInOutBounce(float t, float b, float c, float d);

	/* �J�n�_���I���_�p�^�[�� */
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float LinearFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInQuadFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutQuadFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutQuadFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInCubicFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutCubicFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutCubicFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInQuartFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutQuartFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutQuartFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInQuintFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutQuintFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutQuintFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInSineFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutSineFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutSineFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInExpoFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutExpoFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutExpoFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInCircFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutCircFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutCircFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInElasticFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutElasticFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutElasticFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static float EaseInBackFT(float time, float endTime, float from, float to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static float EaseOutBackFT(float time, float endTime, float from, float to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static float EaseInOutBackFT(float time, float endTime, float from, float to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInBounceFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseOutBounceFT(float time, float endTime, float from, float to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static float EaseInOutBounceFT(float time, float endTime, float from, float to);

	/*** Vector3 ***/

	/* �J�n�_���ړ��ʃp�^�[�� */
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 Linear(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuad(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutCubic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuart(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutQuint(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutSine(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutExpo(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutCirc(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutElastic(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseInBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseOutBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������ / s > 1.0f
	static Vector3 EaseInOutBack(float t, const Vector3& b, const Vector3& c, float d, float s);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInBounce(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseOutBounce(float t, const Vector3& b, const Vector3& c, float d);
	// t = ���� / b = �J�n�l / c = �ړ��� / d = �I������
	static Vector3 EaseInOutBounce(float t, const Vector3& b, const Vector3& c, float d);

	/* �J�n�_���I���_�p�^�[�� */
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 LinearFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInQuadFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutQuadFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutQuadFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInCubicFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutCubicFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutCubicFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInQuartFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutQuartFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutQuartFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInQuintFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutQuintFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutQuintFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInSineFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutSineFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutSineFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInExpoFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutExpoFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutExpoFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInCircFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutCircFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutCircFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInElasticFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutElasticFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutElasticFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static Vector3 EaseInBackFT(float time, float endTime, const Vector3& from, const Vector3& to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static Vector3 EaseOutBackFT(float time, float endTime, const Vector3& from, const Vector3& to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l / s > 1.0f
	static Vector3 EaseInOutBackFT(float time, float endTime, const Vector3& from, const Vector3& to, float s);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInBounceFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseOutBounceFT(float time, float endTime, const Vector3& from, const Vector3& to);
	// time = ���� / endTime = �I������ / from = �J�n�l / to = �I���l
	static Vector3 EaseInOutBounceFT(float time, float endTime, const Vector3& from, const Vector3& to);
};