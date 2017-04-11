#pragma once

#include <string>

struct Vector2;

/// <summary>
/// 二次元座標クラス
/// </summary>
class Point
{
public:
	int x;
	int y;
public:
	/// <summary>
	/// (0, 0)で初期化された二次元座標を返します
	/// </summary>
	static Point Zero;
	/// <summary>
	/// (1, 1)で初期化された二次元座標を返します
	/// </summary>
	static Point One;
	/// <summary>
	/// (-1, 0)で初期化された二次元座標を返します
	/// </summary>
	static Point Left;
	/// <summary>
	/// (1, 0)で初期化された二次元座標を返します
	/// </summary>
	static Point Right;
	/// <summary>
	/// (0, -1)で初期化された二次元座標を返します
	/// </summary>
	static Point Up;
	/// <summary>
	/// (0, 1)で初期化された二次元座標を返します
	/// </summary>
	static Point Down;
	/// <summary>
	/// 画面中央の座標で初期化された二次元座標を返します
	/// </summary>
	static Point Center();
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Point();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Point(int x, int y);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Point(const Point& vec);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit Point(int s);
public:
	/// <summary>
	/// 長さを返します
	/// </summary>
	float Length() const;
	/// <summary>
	/// 長さの二乗を返します
	/// </summary>
	float LengthPow2() const;
	/// <summary>
	/// 二点間の距離を返します
	/// </summary>
	float Distance(const Point& other) const;
public:
	/// <summary>
	/// 二点間の距離を返します
	/// </summary>
	static float Distance(const Point& v1, const Point& v2);

public:
	Point& operator = (const Point& v);
	operator Vector2() const;
	operator std::string() const;
};

bool operator == (const Point& v1, const Point& v2);
bool operator != (const Point& v1, const Point& v2);
Point operator + (const Point& v1, const Point& v2);
Point& operator += (Point& v1, const Point& v2);
Point operator - (const Point& v1, const Point& v2);
Point& operator -= (Point& v1, const Point& v2);
Point operator * (const Point& v, const int f);
Point operator * (const int f, const Point& v);
Point& operator *= (Point& v, const int f);
Point& operator *= (const int f, Point& v);
Point operator / (const Point& v, const int f);
Point operator / (const int f, const Point& v);
Point& operator /= (Point& v, const int f);
Point operator - (const Point& v);
