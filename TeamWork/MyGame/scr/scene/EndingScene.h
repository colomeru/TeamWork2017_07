#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"
#include <vector>

using namespace std;

class EndingScene : public IScene
{
public:
	// �R���X�g���N�^
	EndingScene();
	// �f�X�g���N�^
	~EndingScene();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() const override;
	// �I�����Ă��邩�H
	virtual bool IsEnd() const override;
	// ���̃V�[����Ԃ�
	virtual Scene Next() const override;
	// �I��������
	virtual void End() override;
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

private:
	float size;
	float vertical;
	float horizontal;
	float alpha;
	float distanceH;
	float distanceV;
	float absH;
	float absV;
	float distanceC;
	float cAlpha;
	float aAlpha;
	Vector2 absC;
	Vector2 arrowSize;
	Vector2 particleSize;
	Vector2 arrowPos;
	Vector2 particlePos;
	Vector2 centerPos;
	Vector2 circlePos;
	Vector2 circleSize;
	Vector2 cCenterPos;
	bool turn;
	vector<int> v;
	float friction;
	float x1;
	float x2;

	//��������U��q
	float fx; //�x�_x���W
	float fy; //�x�_y���W
	float rot; //�p�x
	float rot_spd; //�p���x
	float length; //�R�̒���
	float g; //�d�͉����x
	Vector2 spherePos;
	float rad;
	float angle;
	Vector2 line1_Pos_In;
	Vector2 line1_Pos_Out;
	Vector2 line2_Pos_In;
	Vector2 line2_Pos_Out;
	Vector2 line3_Pos_In;
	Vector2 line3_Pos_Out;
	Vector2 line4_Pos_In;
	Vector2 line4_Pos_Out;
	Vector2 line5_Pos_In;
	Vector2 line5_Pos_Out;
	Vector2 line6_Pos_In;
	Vector2 line6_Pos_Out;
	Vector2 line7_Pos_In;
	Vector2 line7_Pos_Out;
	Vector2 line8_Pos_In;
	Vector2 line8_Pos_Out;

	float len;
	float line1_Rot;
	float line2_Rot;
	float line3_Rot;
	float line4_Rot;
	float line5_Rot;
	float line6_Rot;
	float line7_Rot;
	float line8_Rot;
	float rot2;
	
	vector<Vector2> fulcrum;
	int vec;

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;
};