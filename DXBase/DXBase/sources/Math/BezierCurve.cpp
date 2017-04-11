#include "BezierCurve.h"

BezierCurve::BezierCurve()
{
}

void BezierCurve::set(const Vector2 & start, const Vector2 & end, const std::vector<Vector2>& control, const float & time) {
	start_point_ = start;
	end_point_ = end;
	control_points_ = control;
}

void BezierCurve::draw(const int & point_num, Matrix inv) const {
	for (int i = 0; i <= point_num; i++) {
		float t = 1.0f / point_num * i;
		Vector2 pos = get(start_point_, end_point_, control_points_, t);
		Vector3 pos_inv = Vector3(pos.x, pos.y) * inv;
		DrawPixel(static_cast<int>(pos_inv.x), static_cast<int>(pos_inv.y), GetColor(0, 255, 0));
	}

	Vector3 start_inv = Vector3(start_point_.x, start_point_.y) * inv;
	DrawCircle(static_cast<int>(start_inv.x), static_cast<int>(start_inv.y), 5, GetColor(255, 0, 0), TRUE);

	for (unsigned int i = 0; i < control_points_.size(); i++) {
		Vector3 pos_inv = Vector3(control_points_[i].x, control_points_[i].y) * inv;
		DrawCircle(static_cast<int>(pos_inv.x), static_cast<int>(pos_inv.y), 5, GetColor(255, 0, 0), TRUE);
	}

	Vector3 end_inv = Vector3(end_point_.x, end_point_.y) * inv;
	DrawCircle(static_cast<int>(end_inv.x), static_cast<int>(end_inv.y), 5, GetColor(255, 0, 0), TRUE);
} 

Vector2 BezierCurve::get(const Vector2 & start, const Vector2 & end, const std::vector<Vector2>& control, const float & time) const {
	float t = MathHelper::Clamp(time, 0, 1);
	std::vector<Vector2> points = all_points(start, end, control);
	return calc_point(points, t);
}

std::vector<Vector2> BezierCurve::all_points(const Vector2 & start, const Vector2 & end, const std::vector<Vector2>& control) const {
	std::vector<Vector2> points = std::vector<Vector2>();
	points.push_back(start);
	for (unsigned int i = 0; i < control.size(); i++) points.push_back(control[i]);
	points.push_back(end);
	return points;
}

Vector2 BezierCurve::calc_point(const std::vector<Vector2>& points, const float & t) const{
	Vector2 pos = Vector2::Zero;
	for (unsigned int i = 0; i < points.size(); i++) {
		pos += nPasTgl[points.size() - 1][i] * std::pow(1 - t, points.size() - 1 - i) * std::pow(t, i) * points[i];
	}
	return pos;
}
