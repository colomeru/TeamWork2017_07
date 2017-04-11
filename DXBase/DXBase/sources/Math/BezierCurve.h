#pragma once

#include "Vector2.h"
#include "MathHelper.h"
#include "../Actor/Body/Body.h"
#include <vector>

const static int nPasTgl[12][12] = {
	{ 1 },
	{ 1,1 },
	{ 1,2,1 },
	{ 1,3,3,1 },
	{ 1,4,6,4,1 },
	{ 1,5,10,10,5,1 },
	{ 1,6,15,20,15,6,1 },
	{ 1,7,21,35,35,21,7,1 },
	{ 1,8,28,56,70,56,28,8,1 },
	{ 1,9,36,84,126,126,84,36,9,1 },
	{ 1,10,45,120,210,252,210,120,45,10,1 },
	{ 1,11,55,165,330,464,464,330,165,55,11,1 }
};

class BezierCurve {
public:
	BezierCurve();
	void set(const Vector2 &start, const Vector2 &end, const std::vector<Vector2>& control, const float &time);
	void draw(const int & point_num, Matrix inv = Matrix::Identity) const;
private:
	std::vector<Vector2> all_points(const Vector2 & start, const Vector2 & end, const std::vector<Vector2>& control) const;
	Vector2 calc_point(const std::vector<Vector2>& points, const float & t) const;
	Vector2 get(const Vector2 &start, const Vector2 &end, const std::vector<Vector2>& control, const float &time) const;
private:
	Vector2 start_point_;
	Vector2 end_point_;
	std::vector<Vector2>control_points_;
};
