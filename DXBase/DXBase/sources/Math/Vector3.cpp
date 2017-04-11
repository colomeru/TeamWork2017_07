#include "Vector3.h"
#include "MathHelper.h"
#include "Matrix.h"
#include <cmath>
#include <algorithm>

// 定数
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);		// Vector3( 0,  1,  0)
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);		// Vector3( 0, -1,  0)
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);		// Vector3(-1,  0,  0)
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);		// Vector3( 1,  0,  0)
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);	// Vector3( 0,  0, -1)
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);	// Vector3( 0,  0,  1)
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);		// Vector3( 1,  0,  0)
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);		// Vector3( 0,  1,  0)
const Vector3 Vector3::UnitZ(0.0f, 1.0f, 1.0f);		// Vector3( 0,  1,  1)
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);		// Vector3( 1,  1,  1)
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);		// Vector3( 0,  0,  0)

Vector3::Vector3(float x, float y, float z)
	:x(x),y(y),z(z){
}

float Vector3::Length() const{
	return std::sqrt(LengthSquared());
}

float Vector3::LengthSquared() const{
	return Dot(*this, *this);
}

Vector3 & Vector3::Normalize(){
	const float length = Length();
	if (length != 0.0f)*this /= length;
	return *this;
}

Vector3 Vector3::Normalize(const Vector3 & value){
	return Vector3(value).Normalize();
}

float Vector3::Dot(const Vector3 & vector1, const Vector3 & vector2)
{
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

Vector3 Vector3::Cross(const Vector3 & vector1, const Vector3 & vector2)
{
	return Vector3(
		vector1.y * vector2.z - vector1.z * vector2.y,
		vector1.z * vector2.x - vector1.x * vector2.z,
		vector1.x * vector2.y - vector1.y * vector2.x
		);
}

float Vector3::Distance(const Vector3 & vector1, const Vector3 & vector2){
	return (vector2 - vector1).Length();
}

float Vector3::DistanceSquared(const Vector3 & vector1, const Vector3 & vector2){
	return (vector2 - vector1).LengthSquared();
}

Vector3 Vector3::Lerp(const Vector3 & value1, const Vector3 & value2, float amount){
	return Vector3(MathHelper::Lerp(value1.x, value2.x, amount), MathHelper::Lerp(value1.y, value2.y, amount), MathHelper::Lerp(value1.z, value2.z, amount));
}

Vector3 Vector3::Min(const Vector3 & vector1, const Vector3 & vector2){
	return Vector3(std::min<float>(vector1.x, vector2.x), std::min<float>(vector1.y, vector2.y), std::min<float>(vector1.z, vector2.z));
}

Vector3 Vector3::Max(const Vector3 & vector1, const Vector3 & vector2){
	return Vector3(std::max<float>(vector1.x, vector2.x), std::max<float>(vector1.y, vector2.y), std::max<float>(vector1.z, vector2.z));
}

Vector3 Vector3::Clamp(const Vector3 & value1, const Vector3 & min, const Vector3 & max){
	return Vector3(MathHelper::Clamp(value1.x, min.x, max.x), MathHelper::Clamp(value1.y, min.y, max.y), MathHelper::Clamp(value1.z, min.z, max.z));
}

Vector3 Vector3::Transform(const Vector3 & position, const Matrix & matrix){
	return Vector3(
		position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0] + matrix.m[3][0],
		position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1] + matrix.m[3][1],
		position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2] + matrix.m[3][2]
		);
}

Vector3 Vector3::TransformNormal(const Vector3 & position, Matrix & matrix){
	return Vector3(
		position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0],
		position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1],
		position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2]
		);
}

Vector3 Vector3::CreateFromYawPitch(float yaw, float pitch)
{
	return Vector3(
		 MathHelper::Cos(pitch) * MathHelper::Sin(yaw),
		-MathHelper::Sin(pitch),
		 MathHelper::Cos(pitch) * MathHelper::Cos(yaw));
}

float Vector3::Pitch() const{
	float len;
	if ((len = Length()) == 0.0f)return 0.0f;
	return MathHelper::ASin(-y / len);
}

float Vector3::Yaw() const{
	if (Length() == 0.0f)return 0.0f;
	return MathHelper::ATan(x, z);
}

float Vector3::Angle(const Vector3 & from, const Vector3 & to){
	float dot = Dot(Normalize(from), Normalize(to));
	return MathHelper::ACos(MathHelper::Clamp(dot, -1.0f, 1.0f));
}

void Vector3::Spring(Vector3 & pos, Vector3 & resPos, Vector3 & velo, float stiffness, float friction, float mass)
{
	// バネの伸び具合を計算
	Vector3 stretch = (pos - resPos);
	// バネの力を計算
	Vector3 force = -stiffness * stretch;
	// 加速度を追加
	Vector3 acceleration = force / mass;
	// 移動速度を計算
	velo = friction * (velo + acceleration);
	// 座標の更新
	pos += velo;
}

//Dxlib用変換関数
VECTOR Vector3::Vector3ToVECTOR(const Vector3& v) {
	return VGet(v.x, v.y, v.z);
}

// 単項演算子オーバーロード
Vector3 operator - (const Vector3& v){
	return Vector3(-v.x, -v.y, -v.z);
}

// 代入演算子オーバーロード
Vector3& operator += (Vector3& v1, const Vector3& v2){
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}
Vector3& operator -= (Vector3& v1, const Vector3& v2){
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}
Vector3& operator *= (Vector3& v, float s){
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}
Vector3& operator /= (Vector3& v, float s){
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

// ２項演算子オーバーロード
Vector3 operator + (Vector3 v1, const Vector3& v2){
	return v1 += v2;
}
Vector3 operator - (Vector3 v1, const Vector3& v2){
	return v1 -= v2;
}
Vector3 operator * (Vector3 v, float s){
	return v *= s;
}
Vector3 operator * (float s, Vector3 v){
	return v *= s;
}
Vector3 operator / (Vector3 v, float s){
	return v /= s;
}

