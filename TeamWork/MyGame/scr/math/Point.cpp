# include "Point.h"
# include <Dxlib.h>
# include <cmath>
# include "Vector2.h"
# include <sstream>

Point Point::Zero = Point(0, 0);
Point Point::One = Point(1, 1);
Point Point::Left = Point(-1, 0);
Point Point::Right = Point(1, 0);
Point Point::Up = Point(0, -1);
Point Point::Down = Point(0, 1);

Point Point::Center()
{
	int width, height;
	GetWindowSize(&width, &height);
	return Point(width / 2, height / 2);
}

Point::Point()
	: x(0), y(0)
{

}

Point::Point(int x, int y)
	: x(x), y(y)
{

}

Point::Point(const Point& vec)
	: x(vec.x), y(vec.y)
{

}

Point::Point(int s)
	: x(s), y(s)
{

}

float Point::Length() const
{
	return sqrtf(static_cast<float>(x * x + y * y));
}

float Point::LengthPow2() const
{
	return static_cast<float>(x * x + y * y);
}

float Point::Distance(const Point& other) const
{
	return (other - *this).Length();
}

Point& Point::operator = (const Point& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Point::operator Vector2() const
{
	Vector2 v;
	v.x = static_cast<float>(x);
	v.y = static_cast<float>(y);
	return v;
}

Point::operator std::string() const
{
	std::stringstream ss;
	ss << "(" << x << "," << y << ")";
	return ss.str();
}

bool operator == (const Point& v1, const Point& v2)
{
	return (v1.x == v2.x) && (v1.y == v2.y);
}

bool operator != (const Point& v1, const Point& v2)
{
	return (v1.x != v2.x) || (v1.y != v2.y);
}

Point operator + (const Point& v1, const Point& v2)
{
	Point r;
	r.x = v1.x + v2.x;
	r.y = v1.y + v2.y;
	return r;
}

Point& operator += (Point& v1, const Point& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

Point operator - (const Point& v1, const Point& v2)
{
	Point r;
	r.x = v1.x + v2.x;
	r.y = v1.y + v2.y;
	return r;
}

Point& operator -= (Point& v1, const Point& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

Point operator * (const Point& v, const float f)
{
	Point r;
	r.x = static_cast<int>(v.x * f);
	r.y = static_cast<int>(v.y * f);
	return r;
}

Point operator * (const float f, const Point& v)
{
	Point r;
	r.x = static_cast<int>(v.x * f);
	r.y = static_cast<int>(v.y * f);
	return r;
}

Point& operator *= (Point& v, const float f)
{
	v.x = static_cast<int>(v.x * f);
	v.y = static_cast<int>(v.y * f);
	return v;
}

Point& operator *= (const float f, Point& v)
{
	v.x = static_cast<int>(v.x * f);
	v.y = static_cast<int>(v.y * f);
	return v;
}

Point operator / (const Point& v, const int f)
{
	Point r;
	r.x = static_cast<int>(v.x / f);
	r.y = static_cast<int>(v.y / f);
	return r;
}

Point operator / (const int f, const Point& v)
{
	Point r;
	r.x = static_cast<int>(v.x / f);
	r.y = static_cast<int>(v.y / f);
	return r;
}

Point& operator /= (Point& v, const float f)
{
	v.x = static_cast<int>(v.x / f);
	v.y = static_cast<int>(v.y / f);
	return v;
}

Point operator - (const Point& v)
{
	Point r = v;
	r.x = -r.x;
	r.y = -r.y;
	return r;
}