#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"

class Player_Head;
class Player_Sword;

static const Vector2 pHDist = Vector2(32, 32);
static const int headAngleSetter = -2;
static const float defHeadChangeTime = 0.2f;
static const float defHeadLength = 2.f;
static const float defPGravPow = 0.05f;
static const float defGravAddPow = 0.2f;
static const float HeadShootMult = 0.5f;
static const float defSlipCount = 8.f;
static const int defLaneChangeCoolTime_ = 60;
static const int defChainLockCoolTime_ = 10;
//���������position���Z�l
static const float fallAddPosMult = 3.4f;
//������܂ł̍ő厞��
static const float defResistTime = 0.5f;
//���[���̖{��

enum {
	MODE_FALL = 0,
	MODE_SHOOT = 1,
	MODE_SHOOT_END = 2,
	MODE_BITE = 3,
	MODE_SLIP = 4,
	MODE_RESIST = 5,
	MODE_CLEAR = 6,
	MODE_PLAYERDEAD = 7
};


class Player : public Actor, public std::enable_shared_from_this<Player>
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//�R���X�g���N�^
	Player(IWorld* world,int maxLaneSize=3,int startLane=1);
	//�f�X�g���N�^
	~Player();
	//�X�V
	virtual void Update() override;
	virtual void FastUpdate() override {
		if (!world_->GetIsCamChangeMode()) {
			int nexLane = world_->GetKeepDatas().nextLane_;
			UpdateLaneNum(nexLane, changeType_);
			world_->GetCanChangedKeepDatas().SetPlayerNextLane(0);
		}

	}
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	virtual bool CamMoveUpdate() {
		if (world_->GetKeepDatas().nextLane_ < 0) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}

		return true;
	}
	virtual void CamMoveUp()override {
	}
	virtual void CamMoveDown() override{
		if (changeType_ == LaneChangeType::LaneChange_Normal) {
			return;
		}
		//drawPos_ = GetDrawPosVect(position_);
		LaneChangeFall();
	}
	virtual void LaneChangeFall() override {

		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

		if (changeType_ == LaneChangeType::LaneChange_Fall) {
			//drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
			drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
		}
	}
	bool isLaneChangeFall() const{
		return changeType_ == LaneChangeType::LaneChange_Fall;
	}
	//�U��q�^��
	void Pendulum(Vector2 fulcrum, float length);
	void StartPendulum();

	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	//Head�̒��������ۂ̃Q�[���ɔ��f�����l�ɕϊ����ĕԂ�
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	//���݂�Head�̎�̒�����Ԃ�
	Vector2 GetCurrentHeadLength()const;
	void HeadPosUpdate()
	{
		headChangeTime_ -= 0.016f*sign(headChangeTime_);

		rotTimer = 0;
		if (MathHelper::Abs(headChangeTime_) <= 0.01f)headChangeTime_ = 0;
		else if (MathHelper::Abs(headChangeTime_) > 0)rotTimer = headChangeTime_ * 5;//MathHelper::Abs(defHeadChangeTime/1.f);

		rotTimer+=clearAddRot_;
		for (int i = 0; i < pHeadPoses_.size(); i++) {
			Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((i + headAngleSetter - currentHead_) * 45)/*+angle_*/ + ((rotTimer)* 45));
			Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
			pHeadPoses_[i] = cgToV2;
		}

	}
	void SwordPosUpdate();
	//�g�p���铪���E�ׂ̕��ɕύX
	void changeHead() {
		//��]�������_��Slip��Ԃ𒼂�
		isSlipped_ = false;
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_++;
		if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	//�g�p���铪�����ׂ̕��ɕύX
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
	void CurHeadBite(const Vector2& target);
	void ResurrectHead();
	void SetOtherClothesID_(CLOTHES_ID cId) {
		otherClothesID_ = cId;
	}
	//���ݕt�����Ԃ���Ԃ�(���W�X�g�܂�)
	bool GetIsShootMode()const {
		return playerMode_ == MODE_SHOOT;
	}
	bool GetIsBiteMode()const {
		return playerMode_ == MODE_BITE || playerMode_ == MODE_RESIST;
	}
	bool GetIsResistMode()const {
		return playerMode_ == MODE_RESIST;
	}
	bool GetIsClearMode()const {
		return playerMode_ == MODE_CLEAR;
	}
	int GetPHeadLiveCount()const {
		int result = 0;
		for (auto i : pHeadDead_) {
			if (!i)result++;
		}
		return result;
	}
	//���ݕt����Ԃɂ��邩���Z�b�g�A
	void SetIsBiteMode(bool ismode) {
		int setMode = (ismode) ? MODE_BITE : MODE_SHOOT;
		playerMode_ = setMode;
	}
	void SetMode(int pMode);
	//�V���[�g�I���̏u�Ԃ��ǂ��������
	bool GetIsShootModeEnd()const {
		return playerMode_==MODE_SHOOT_END;
	}
	float GetSlipCount()const {
		return slipCount_;
	}
	bool GetIsSlipped()const {
		return playerMode_==MODE_SLIP;
	}
	//Head�̃��[����{�̂̃��[���ɍ��킹��
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//world�̋��L�f�[�^�Ɏ����̏���������
	void worldSetMyDatas();

	//void SetIsCanChangeLane(bool isCanChange) {
	//	if (laneChangeCoolTime_ > 0)return;
	//	laneChangeCoolTime_ = defLaneChangeCoolTime_;
	//	isCanChangeLane_ = isCanChange;
	//}
	void SetNextLane(int addNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	void setCurPHeadSPos(const Vector2& sPos) {
		if (isTutorialText_) return;

		//pHeads_[currentHead_]->setPHeadStopPos(sPos);
		SetMultiplePos(sPos - stopPos_);
		stopPos_ = sPos;
		//Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
		//MultipleInit(lngPs.Length(), stopPos_, MathAngle(position_ - pHeadPoses_[currentHead_], Vector2::Down));

	}
	void setMaxLaneSize(int size) {
		maxLaneSize_ = size;
	}
	//���݂̓��ɑ΂��Ċ��邩�ǂ������Z�b�g����
	void curPHeadSlip(bool isSlip);
	//�v���C���[������ł邩
	bool isPlayerDead()const {
		if(laneNum_==(maxLaneSize_-1)&&position_.y >= WINDOW_HEIGHT-200)return true;
		//if (position_.y >= WINDOW_HEIGHT)return true;

		for (auto pHD : pHeadDead_) {
			if (!pHD)return false;
		}
		return true;
	}
	void PHeadChanger(int rot = 0);
	void SetStopPos(Vector2 target);
	Vector2 GetStopPos()const {
		return stopPos_;
	}
	float GetRot()const {
		return mRot.front();
	}
	Vector2 GetHeadPosAddVect() const{
		return headPosAddVect_;
	}
	Vector2 GetSlipHeadPoint()const{
		return fPos_.front();
	}
	//���ݎg�p���Ă���Head�̍��W��Ԃ�
	Vector2 GetCurrentPHeadPosition()const;
	float GetPlayerSwordAngle()const;
	//�U��q�̎x�_�ʒu���ړ�����
	void SetMultipleFulcrumPos(const Vector2& pos) {
		fPos_.front() = pos;
	}
	bool GetIsSwordActive()const;
	bool GetIsClearBite()const;
	bool GetIsClearShoot()const;
	Actor* GetCurrentHead() const;
	void deadLine();
	void SetUseKey(bool key) {
		isUseKey_ = key;
	}
	bool GetUseKey()const { return isUseKey_; }
	void SetIsTutorialTextWriting(bool is) {
		isTutorialText_ = is;
	}
private:
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	void Multiple();
	//���d�U��q�Ɉړ��ʂ����Z
	void UpdateMultiplePos();
	void SetMultiplePos(const Vector2& addpos);
	//���d�U��q�������I�Ɉړ�
	void AddMultiplePos(const Vector2& addPos) {
		for (int i = 0; i < multiplePos.size(); i++) {
			multiplePos[i] += (addPos);
			if (i > 0) fPos_[i] = multiplePos[i - 1];
		}
	}
	void SetNeckNonMult();
	void DeformationDraw();
	//��̕`��ɕK�v�Ȉ�A�̓�����s��
	void SetDrawNeck(const Vector2& bodyPoint,const Vector2& headPoint);
	//��̕`��ʒu��ݒ�
	void SetDrawPoint(const Vector2& bodyPoint, const Vector2& headPoint);
	//��̊e�`��ʒu��ݒ肷��
	void SetDrawNeckParts(const Vector2& bodyPoint, const Vector2& headPoint);
	//���͂ɂ�铮����܂Ƃ߂�
	void PlayerInputControl();
	//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����

	void PHeadLengthReset() {
		//�����̕�Ԃ����Z�b�g����
		chainAddLength_ = 0.f;
		chainAddLengthMath_ =0.f;

		//�`�F�[���̃��b�N�����Z�b�g����
		chainLock_ = false;
		for (auto& pHL : pHeadLength_) {
			pHL = 2.f;
		}
	}
	//�`�F�[���̒��������Z����
	void CurPHeadLengPlus(float addPow);
	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal) {
		if (updateNum == 0)return;
		if (laneNum_ + updateNum > (maxLaneSize_ - 1) || laneNum_ + updateNum < 0)return;

		//���̃��[���ɑΉ������x�N�g�����쐬���A�d�͂̉��Z�����Z�b�g����
		Vector2 nextVel_;
		//�オ��Ƃ�
		if (updateNum < 0) {
			nextVel_ = Vector2(0, -15.f);
			pGrav_ = 0.f;
			//position_.y += defDrawLinePosY[2]- defDrawLinePosY[1];
			
			world_->sendMessage(EventMessage::LANE_CHANGE_UP_END);
		}
		//�~��鎞
		else if (updateNum > 0) {
			if (changeType == LaneChangeType::LaneChange_Fall) {
				nextVel_ = pendulumVect_ / 3;
				pGrav_ *= 0.1f;
			}
		}
	
		laneNum_ += updateNum;
		//���[���ő�͈͂𒴂�����Vect�̕␳���s��Ȃ�

		laneNum_ = MathHelper::Clamp(laneNum_, 0, (maxLaneSize_ - 1));

		//velocity_ = nextVel_;
		pendulumVect_ = nextVel_;

		if(GetIsBiteMode())playerMode_ = MODE_SLIP;
		//���̒��������Z�b�g
		//PHeadChanger();
		//PHeadLengthReset();

		world_->sendMessage(EventMessage::LANE_CHANGE_END);

		worldSetMyDatas();
	}
	void CreateBiteEffect();


//�v���C���[�̏�Ԃɉ������X�V
private:
	void FallUpdate();
	void ShootUpdate();
	void ShootEndUpdate();
	void BiteUpdate();
	void SlipUpdate();
	void ResistUpdate();
	void ClearUpdate();
	void DeadUpdate();
private:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	using PSwordPtr = std::shared_ptr<Player_Sword>;
	//�X�e�[�W�̍ő僌�[����
	int maxLaneSize_;

	int addscorelist_[3];

	float spdLimit;

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
	//�U��q�ړ��̖��C
	float friction;
	//�U��q�ړ��ɂ��x�N�g�������o��
	Vector2 pendulumVect_;

	//���d�U��q
	std::vector<Vector2> fPos_;
	std::vector<Vector2> multiplePos;
	std::vector<float> mRot;
	std::vector<float> mRot_spd;
	std::vector<float> correctionLens;
	std::vector<DrawPos> drawPoints;



	//Head���Î~����ʒu���i�[����
	Vector2 stopPos_;

	PSwordPtr pSword_;

	std::vector<PHeadPtr> pHeads_;
	std::vector<Vector2> pHeadPoses_;
	//�eHead�̃`�F�[���̒���
	std::vector<float> pHeadLength_;
	//�񂪊O�ꂽ���ɒǉ��ŐL�т�`�F�[���̒���
	float chainAddLength_;
	//���ǉ��ŐL�΂��ۂ̕�Ԓl
	float chainAddLengthMath_;
	std::vector<bool> pHeadDead_;

	int currentHead_;

	float headChangeTime_;

	float pGrav_;

	float rotTimer;
	float clearAddRot_;

	float maxChainLength_;

	float slipCount_;

	bool isSlipped_;

	bool isPlayerFallLane_;

	Vector2 headPosAddVect_;

	//���藎����܂ł̎���
	float slipResistTime_;

	//0=�؋� 1=���ˎ� 2=���ˏI�� 3=���ݕt�� 4=���藎��
	int playerMode_;
	//�L�[���b�N
	bool isNextPushKey_;
	float jumpShotPower_;
	bool chainLock_;

	//���[���ړ������t���[���Ŋ��ޑΏۂ����邩�𒲂ׂ�
	bool isNextLaneBite_;

	int laneAddNum_;
	//bool isCanChangeLane_;

	int chainLockCoolTime_;
	
	//Head��]�����b�N����(�X�e�B�b�N��0�ɖ߂����Ń��Z�b�g)
	bool isCanNextHeadRot;

	bool isClearShoot_;

	bool isUseKey_;
	bool isTutorialText_;
	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;
	//���鎞�Ԃ̔{��(����)
	std::map<CLOTHES_ID, float> slipCountMult_;

	std::map<int, std::function<void()>> updateFunctionMap_;
};

