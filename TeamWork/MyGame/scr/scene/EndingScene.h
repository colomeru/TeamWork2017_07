#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"
#include <vector>
#include <array>
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "../scene/addScreen/screenSupport/ProgressMeter.h"

using namespace std;

class EndingScene : public IScene
{
public:
	struct DrawPos {
		Vector2 p0;
		Vector2 p1;
		Vector2 p2;
		Vector2 p3;
	};

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
	////�P�U��q
	//void Pendulum();
	////��d�U��q
	//void Double(Vector2 fPos);
	//���d�U��q
	void Multiple();
	//�ό`�`��
	void DeformationDraw();
	//���d�U��q������
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius); //��̒����A�x�_�A�p�x�A���a

private:
	//float size;
	//float vertical;
	//float horizontal;
	//float alpha;
	//float distanceH;
	//float distanceV;
	//float absH;
	//float absV;
	//float distanceC;
	//float cAlpha;
	//float aAlpha;
	//Vector2 absC;
	//Vector2 arrowSize;
	//Vector2 particleSize;
	//Vector2 arrowPos;
	//Vector2 particlePos;
	//Vector2 centerPos;
	//Vector2 circlePos;
	//Vector2 circleSize;
	//Vector2 cCenterPos;
	bool turn;
	vector<int> v;
	float friction; //���C

	//��������U��q
	//float fx; //�x�_x���W
	//float fy; //�x�_y���W
	//float rot; //�p�x
	//float rot_spd; //�p���x
	float length; //�R�̒���
	float g; //�d�͉����x
	Vector2 spherePos; //�d��̈ʒu

	float len; //��̒���
	float rot2;
	float r; //�d��̔��a
	float stageLen; //�X�e�[�W�̒���
	float meterLen; //�i�s�x���[�^�[�̒���
	Vector2 meterPos; //�i�s�x���[�^�[�̈ʒu
	StageGenerateManager stageManager;

	std::array<Vector2, 8> fulcrum; //�x�_
	std::array<Vector2, 8> inPos; //����
	std::array<Vector2, 8> outPos; //�O��
	std::array<float, 8> lineRot; //���ꂼ��̎�̊p�x
	std::array<float, 8> neckLen; //���ꂼ��̎�̒���
	int vec; //���݂̎�ԍ�
	//const float spdLimit = 2.75f; //
	float spdLimit; //
	bool rotDirection;; //��]�����Ftrue�Ȃ�E���Afalse�Ȃ獶���
	int r1; //1�t���[���O��rot
	int r2; //1�t���[�����rot

	Vector2 v1;
	Vector2 v2;
	float f;

	//2�d�U��q
	Vector2 doublePos;
	float dRot;
	float dRot_spd;
	float dFriction;
	float dSub;

	float any; //�f�o�b�O�\���p
	float any1;
	float any2;
	Vector2 anyPos1;
	array<Vector2, 8> anyPos2;

	//���d�U��q
	int fNum;
	float mFriction;
	float mSub;
	std::vector<Vector2> fPos_;
	std::vector<Vector2> multiplePos;
	std::vector<float> mRot;
	std::vector<float> mRot_spd;
	std::vector<float> mLimit;
	std::vector<float> correctionLens;
	float oneLength;


	//�ό`�`��
	int hNeck, hHead;
	std::array<Vector2, 4> gPos1;
	std::array<Vector2, 4> gPos2;
	int posNum;
	Vector2 pos[7][4];
	float neckLengh;
	std::vector<DrawPos> drawPoints;

	//

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;

	using PlayerPtr = std::shared_ptr<Player>;
	PlayerPtr player_;

	ProgressMeter meter_;
};