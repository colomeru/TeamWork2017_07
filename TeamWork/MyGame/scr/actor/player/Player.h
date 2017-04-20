#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"

class Player_Head;

static const Vector2 pHDist = Vector2(32, 32);
static const int headAngleSetter = -2;
static const float defHeadChangeTime = 0.2f;
static const float defHeadLength = 2.f;
static const float defPGravPow = 0.07f;
static const float defGravAddPow = 0.1f;
static const float HeadShootMult = 0.5f;
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	//�R���X�g���N�^
	Player(IWorld* world);
	//�f�X�g���N�^
	~Player();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	//�U��q�^��
	void Pendulum(Vector2 fulcrum, float length);
	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	float GetHeadLength(int headNum) const {
		return pHeadLength_[headNum];
	}
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	void HeadPosUpdate();
	void changeHead() {
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_++;
		if (currentHead_ >= 8)currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	void backChangeHead() {
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_--;
		if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
		headChangeTime_ = -defHeadChangeTime;
	}

	int GetCurHead()const {
		return currentHead_;
	}
	
	void CurHeadBite(const Vector2& target) {
		pGrav_ = defPGravPow;
		Vector2 tpos = target - position_;
		//�p�x�����߂�
		//rot_ = MathHelper::ACos(Vector2::Dot(Vector2::Right, tpos)) *180 / MathHelper::Pi;
		rot_ = 90;
		rot_spd_ = 3.0f;
	}
	//���ݕt�����Ԃ���Ԃ�
	bool GetIsBiteMode()const {
		return isBiteMode_;
	}
	//Head�̃��[����{�̂̃��[���ɍ��킹��
	void SetMyHeadLaneNum(int targetNum);
private:
	//���͂ɂ�铮����܂Ƃ߂�
	void PlayerInputControl();

	void PHeadLengthReset() {
		for (auto& pHL : pHeadLength_) {
			pHL = 2.f;
		}
	}
	void CurPHeadLengPlus(float addPow);
private:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	
	//�Փ˂��Ă��邩
	bool isHit_;

	//�U��q�֘A
	//�x�_���W
	Vector2 fulcrum_;
	//�p�x
	float rot_;
	//�p���x
	float rot_spd_;
	//�R�̒���
	float length_;
	//�d�͉����x
	float gravity_;
	//�U��q�ړ��ɂ��x�N�g�������o��
	Vector2 pendulumVect_;

	std::vector<PHeadPtr> pHeads_;
	std::vector<Vector2> pHeadPoses_;
	//�eHead�̃`�F�[���̒���
	std::vector<float> pHeadLength_;

	int currentHead_;

	float headChangeTime_;

	float pGrav_;

	float rotTimer;

	float maxChainLength_;

	bool isBiteMode_;

	bool isShootMode_;
	bool isNextPushKey_;
};