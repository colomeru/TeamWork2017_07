#pragma once
#include<vector>


struct Vector2;
struct NeckPiecePoint;

class PlayerNeckPendulumSupport {
public:
	//始点、終点から首を生成する
	static void Init(const Vector2& fulcrum, const Vector2& tipPos,
		std::vector<Vector2>& fulcrum_List,
		std::vector<Vector2>& tipPos_List,
		std::vector<float>& rotList,
		std::vector<float>& rot_Speed_List,
		std::vector<NeckPiecePoint>& neck_Point_List,
		float rot_speed = 1.0f);
	//振り子の点の位置を更新する、最終点の移動量を返す
	static Vector2 Pendulum(
		std::vector<Vector2>& fulcrum_List,
		std::vector<Vector2>& tipPos_List,
		std::vector<float>& rot_List,
		std::vector<float>& rot_Speed_List,
		std::vector<NeckPiecePoint>& neck_Point_List
		);

private:
	static void Deformation(
		const std::vector<Vector2>& fulcrum_List,
		const std::vector<Vector2>& tipPos_List,
		std::vector<NeckPiecePoint>& neck_Point_List
		);
};