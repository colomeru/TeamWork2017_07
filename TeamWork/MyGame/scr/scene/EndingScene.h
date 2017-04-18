#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"

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
	Vector2 linePos_1;
	Vector2 linePos_2;
	float len;
	float lineRot;
	float rot2;


private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;
};