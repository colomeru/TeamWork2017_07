#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

// マスヘルパークラス
class MathHelper {
public:
	// πの値を表します。
	static const float Pi;

	// 値を指定された範囲内に制限します
	static float Clamp(float value, float min, float max);
	// 2つの値の差の絶対値を計算します
	static float Distance(float n1, float n2);
	// 2つの値の間を線形補間します
	static float Lerp(float value1, float value2, float amount);
	// ラジアンを度に変換します。
	static float ToDegrees(float radians);
	// 度をラジアンに変換します。
	static float ToRadians(float degree);

	static float Sin(float degree);

	static float Cos(float degree);

	static float Tan(float degree);

	static float ASin(float s);

	static float ACos(float c);

	static float ATan(float y, float x);
	// 変数の符号を受け取る
	static int Sign(float value);
	//バネ補正
	static void Spring(float & num, float & resNum, float & velo, float stiffness, float friction, float mass);
	static void SpringInt(int & num, int & resNum, int & velo, float stiffness, float friction, float mass);
};

#endif
