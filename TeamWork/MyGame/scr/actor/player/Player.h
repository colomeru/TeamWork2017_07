#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
class Player_Head;

static const Vector2 pHDist = Vector2(32, 32);
static const int headAngleSetter = -2;
static const float defHeadChangeTime = 0.2f;
static const float defHeadLength = 2.f;
static const float defPGravPow = 0.05f;
static const float defGravAddPow = 0.2f;
static const float HeadShootMult = 0.5f;
static const float defSlipCount = 5.f;
static const int defLaneChangeCoolTime_ = 60;


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
	//Head�̃f�[�^��̒�����Ԃ�
	float GetHeadLength(int headNum) const {
		return pHeadLength_[headNum];
	}
	//Head�̒��������ۂ̃Q�[���ɔ��f�����l�ɕϊ����ĕԂ�
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	void HeadPosUpdate();
	void changeHead() {
		//��]�������_��Slip��Ԃ𒼂�
		isSlipped_ = false;
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
	//�v���C���[�̊J�n���̏�Ԑݒ�
	void StartPlayerSet();
	int GetCurHead()const;
	bool GetPHeadDead(int pHeadNum)const {
		return pHeadDead_[pHeadNum];
	}
	void CurHeadBite(const Vector2& target) {
		isBiteMode_ = true;
		pGrav_ = defPGravPow;
		//�p�x�����߂�
		//rot_ = MathHelper::ACos(Vector2::Dot(Vector2::Right, tpos)) *180 / MathHelper::Pi;
		rot_ = 135;
		rot_spd_ = -3.0f;
	}
	void SetOtherClothesID_(CLOTHES_ID cId) {
		otherClothesID_ = cId;
	}
	//���ݕt�����Ԃ���Ԃ�
	bool GetIsBiteMode()const {
		return isBiteMode_;
	}
	void SetIsBiteMode(bool ismode) {
		isBiteMode_ = ismode;
	}
	int GetIsShootMode()const {
		return isShootMode_;
	}
	float GetSlipCount()const {
		return slipCount_;
	}
	bool GetIsSlipped()const {
		return isSlipped_;
	}
	//Head�̃��[����{�̂̃��[���ɍ��킹��
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//world�̋��L�f�[�^�Ɏ����̏���������
	void worldSetMyDatas();

	//�V�������[���̒l����͂��āA���̃��[���ɍs���邩�𒲂ׂ�
	void SetNextLane(int updateNum) {
		if (laneChangeCoolTime_ > 0)return;

		laneAddNum_ = updateNum;
		nextLane_ = laneNum_ + updateNum;

	}
	void SetIsCanChangeLane(bool isCanChange) {
		if (laneChangeCoolTime_ > 0)return;
		laneChangeCoolTime_ = defLaneChangeCoolTime_;
		isCanChangeLane_ = isCanChange;
	}
	bool GetIsReSetClothesType_()const {
		return isReSetClothesType_;
	}
	void SetIsReSetClothesType_(bool isReset) {
		isReSetClothesType_ = isReset;
	}
private:
	//���͂ɂ�铮����܂Ƃ߂�
	void PlayerInputControl();

	void PHeadLengthReset() {
		//�`�F�[���̃��b�N�����Z�b�g����
		chainLock_ = false;
		for (auto& pHL : pHeadLength_) {
			pHL = 2.f;
		}
	}
	//�`�F�[���̒��������Z����
	void CurPHeadLengPlus(float addPow);

	void UpdateLaneNum(int updateNum) {
		isCanChangeLane_ = false;

		laneNum_ += updateNum;
		laneNum_ = MathHelper::Clamp(laneNum_, 0, 2);

		//�ړ���̕��̎�ނɍĐݒ肷��
		isReSetClothesType_ = true;

		worldSetMyDatas();
	}
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
	std::vector<bool> pHeadDead_;

	int currentHead_;

	float headChangeTime_;

	float pGrav_;

	float rotTimer;

	float maxChainLength_;

	float slipCount_;

	bool isSlipped_;

	//����ł��邩
	bool isBiteMode_;

	//0=false 1=start 2=end
	int isShootMode_;
	bool isNextPushKey_;
	float jumpShotPower_;
	bool chainLock_;

	//���[���ړ������t���[���Ŋ��ޑΏۂ����邩�𒲂ׂ�
	bool isNextLaneBite_;

	int nextLane_;
	int laneAddNum_;
	bool isCanChangeLane_;

	bool isReSetClothesType_;

	int laneChangeCoolTime_;

	CLOTHES_ID otherClothesID_;

	//���鎞�Ԃ̔{��(����)
	std::map<CLOTHES_ID, float> slipCountMult_;

};