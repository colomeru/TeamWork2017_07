#pragma once
#include<vector>


struct Vector2;
struct NeckPiecePoint;

class PlayerNeckPendulumSupport {
public:
	//�n�_�A�I�_�����𐶐�����
	static void Init(const Vector2& fulcrum, const Vector2& tipPos,
		std::vector<Vector2>& fulcrum_List,
		std::vector<Vector2>& tipPos_List,
		std::vector<float>& rotList,
		std::vector<float>& rot_Speed_List,
		std::vector<NeckPiecePoint>& neck_Point_List,
		float rot_speed = 1.0f);
	//�U��q�̓_�̈ʒu���X�V����A�ŏI�_�̈ړ��ʂ�Ԃ�
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