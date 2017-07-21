#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include"PlayerNeck\NeckPiecePoint.h"
#include"../../method/MethodTimer.h"
#include"Input/PlayerInputChecker.h"

class Player_Head;
class Player_Sword;

static const Vector2 pHDist = Vector2(32, 32);
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

enum Player_Mode{
	MODE_FALL = 0,
	MODE_SHOOT = 1,
	MODE_SHOOT_END = 2,
	MODE_BITE = 3,
	MODE_SLIP = 4,
	MODE_RESIST = 5,
	MODE_CLEAR = 6,
	MODE_PLAYERDEAD = 7,
	MODE_BACK_SHOOT=8,
};


class Player : public Actor, public std::enable_shared_from_this<Player>
{

protected:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//�R���X�g���N�^
	Player(IWorld* world,int maxLaneSize=3,int startLane=1,const Vector2& position=Vector2(0.f,200.f));
	//�f�X�g���N�^
	~Player();
	//�X�V
	virtual void Update() override;
	//���O�X�V
	virtual void FastUpdate() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	//���[���ړ����X�V
	virtual bool CamMoveUpdate() {
		if (world_->GetKeepDatas().nextLane_ < 0) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}

		return true;
	}
	//��ړ�(������)
	virtual void CamMoveUp()override {
	}

	//���ړ�
	virtual void CamMoveDown() override{
		if (changeType_ == LaneChangeType::LaneChange_Normal) {
			return;
		}
		LaneChangeFall();
	}
	//����
	virtual void LaneChangeFall() override;
	//�����ɂ�郌�[���ړ����𒲂ׂ�
	bool isLaneChangeFall() const{
		return changeType_ == LaneChangeType::LaneChange_Fall;
	}
	//�U��q�J�n
	void StartPendulum();
	//��̍��{�̈ʒu�𒲂ׂ�
	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	//���݂̎�̍��{�̈ʒu�𒲂ׂ�
	Vector2 GetHeadPos()const {
		return pHeadPoses_[currentHead_];
	}
	//Head�̒��������ۂ̃Q�[���ɔ��f�����l�ɕϊ����ĕԂ�
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	//���݂�Head�̎�̒�����Ԃ�
	Vector2 GetCurrentHeadLength()const;
	//�S�Ă̓��̈ʒu���X�V����
	void HeadPosUpdate();
	//���̈ʒu���X�V����
	void SwordPosUpdate();
	//�g�p���铪���E�ׂ̕��ɕύX
	void changeHead() {
		//��]�������_��Slip��Ԃ𒼂�
		currentHead_++;
		if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	//�g�p���铪�����ׂ̕��ɕύX
	void backChangeHead() {
		currentHead_--;
		if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
		headChangeTime_ = -defHeadChangeTime;
	}
	//�v���C���[�̊J�n���̏�Ԑݒ�
	void StartPlayerSet();
	//���ݎg���Ă��铪��ID���擾
	int GetCurHead()const;
	//�w��ID�̓�������ł��邩�𒲂ׂ�
	bool GetPHeadDead(int pHeadNum)const {
		return pHeadDead_[pHeadNum];
	}
	//���ݕt�����J�n����
	void CurHeadBite(const Vector2& target);
	//����1�h������
	bool ResurrectHead();
	//�͂�ł��镞�̎�ނ�ݒ肷��
	void SetOtherClothesID_(CLOTHES_ID cId) {
		otherClothesID_ = cId;
	}
	//���ݕt�����Ԃ���Ԃ�(���W�X�g�܂�)
	bool GetIsShootMode()const {
		return playerMode_ == MODE_SHOOT;
	}
	//���ݕt�����or���񒣂��Ԃ��𒲂ׂ�
	bool GetIsBiteMode()const {
		return playerMode_ == MODE_BITE || playerMode_ == MODE_RESIST;
	}
	//�v���C���[�̏�Ԃ��`�F�b�N����A�����̃��[�h�ƈ�v���Ă����true
	bool PlayerModeChecker(Player_Mode pMode) {
		return playerMode_ == pMode;
	}
	//���񒣂��Ԃ��𒲂ׂ�
	bool GetIsResistMode()const {
		return playerMode_ == MODE_RESIST;
	}
	//�X�e�[�W�N���A��Ԃ��𒲂ׂ�
	bool GetIsClearMode()const {
		return playerMode_ == MODE_CLEAR;
	}
	//���ݐ����Ă��铪�̐��𒲂ׂ�
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
	//�v���C���[�̏�Ԃ�ύX����
	void SetMode(Player_Mode pMode, bool isPlaySE=true);
	//�V���[�g�I���̏u�Ԃ��ǂ��������
	bool GetIsShootModeEnd()const {
		return playerMode_==MODE_SHOOT_END;
	}
	//���藎����܂ł̎��Ԃ�Ԃ�
	float GetSlipCount()const {
		return slipCount_;
	}
	//���藎����Ԃ��𒲂ׂ�
	bool GetIsSlipped()const {
		return playerMode_==MODE_SLIP;
	}
	//�ҏW�\�C���v�b�g���擾����
	PlayerInputChecker& GetEditableUseKey() {
		return isUseKey_;
	}
	std::vector<float>& GetEditableRot_Speed() {
		return mRot_spd;
	}
	//Head�̃��[����{�̂̃��[���ɍ��킹��
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//world�̋��L�f�[�^�Ɏ����̏���������
	void worldSetMyDatas();
	//���Ɉړ����郌�[����ݒ肷��
	void SetNextLane(int addNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//�x�_�Œ���W��ݒ肵�A��̈ʒu��␳����
	void setCurPHeadSPos(const Vector2& sPos) {
		if (isTutorialText_) return;

		SetMultiplePos(sPos - stopPos_);
		stopPos_ = sPos;
	}
	//�U��q�ɂ����o���ꂽ�ړ��x�N�g�����擾����
	Vector2 GetPendulumVect()const {
		return pendulumVect_;
	}
	//�U��q�ɂ����o���ꂽ�ړ��x�N�g�����㏑������
	void SetPendulumVect(const Vector2& pvect) {
		pendulumVect_ = pvect;
	}
	//���݂̓��ɑ΂��Ċ��邩�ǂ������Z�b�g����
	void curPHeadSlip(bool isSlip);
	//�v���C���[������ł邩
	bool isPlayerDead()const {
		if(laneNum_==(maxLaneSize_-1)&&position_.y >= WINDOW_HEIGHT-200)return true;

		for (auto pHD : pHeadDead_) {
			if (!pHD)return false;
		}
		return true;
	}
	//��̒��������Z�b�g���A������]������܂ł̈�A�̓�����s��(1�ŉE��]�A�f�t�H���g�ō���])
	void PHeadChanger(int rot = 0);
	//�x�_�Œ���W��ݒ肷��
	void SetStopPos(Vector2 target);
	//�x�_�Œ���W���擾����
	Vector2 GetStopPos()const {
		return stopPos_;
	}
	//��̐�[�̊p�x�𒲂ׂ�
	float GetRot()const {
		return mRot.front();
	}
	//��̍��{�̊p�x�𒲂ׂ�
	float GetRotBack()const {
		return mRot.back();
	}
	//���藎������Head�̂���ׂ��ʒu���擾����
	Vector2 GetSlipHeadPoint()const{
		return fPos_.front();
	}
	//���ݎg�p���Ă���Head�̍��W��Ԃ�
	Vector2 GetCurrentPHeadPosition()const;
	//
	float GetPlayerSwordAngle()const;
	//�U��q�̎x�_�ʒu���ړ�����
	void SetMultipleFulcrumPos(const Vector2& pos) {
		fPos_.front() = pos;
	}
	//���݌����L�����𒲂ׂ�
	bool GetIsSwordActive()const;
	//�N���A���̐U��q��Ԃ��𒲂ׂ�
	bool GetIsClearBite()const;
	//�N���A���̔��ˏ�Ԃ��𒲂ׂ�
	bool GetIsClearShoot()const;
	//���݂̓��𒲂ׂ�
	Actor* GetCurrentHead() const;
	//�v���C���[�̈ʒu�ɉ�������ԑJ�ڂ��s��(�V�[�����ŗL���A���������肷��)
	void deadLine();
	//�L�[��L�������邩�ǂ��������肷��
	void SetUseKey(bool key) {
		isUseKey_.SetUseKey(key);
	}
	bool GetUseKey()const { return isUseKey_.GetUseKey(); }
	void SetIsTutorialTextWriting(bool is) {
		isTutorialText_ = is;
	}
	bool IsLookBack()const;
protected:
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	void Multiple();
	//���d�U��q�Ɉړ��ʂ����Z
	void UpdateMultiplePos();
	void SetMultiplePos(const Vector2& addpos);
	//���d�U��q�������I�Ɉړ�
	void AddMultiplePos(const Vector2& addPos) {
		for (int i = 0; i < (int)multiplePos.size(); i++) {
			multiplePos[i] += (addPos);
			if (i > 0) fPos_[i] = multiplePos[i - 1];
		}
	}
	//��̕`��ɕK�v�Ȉ�A�̓�����s��
	void SetDrawNeck(const Vector2& bodyPoint,const Vector2& headPoint);
	//��̕`��ʒu��ݒ�
	void SetDrawPoint(const Vector2& bodyPoint, const Vector2& headPoint);
	//��̊e�`��ʒu��ݒ肷��
	void SetDrawNeckParts(const Vector2& bodyPoint, const Vector2& headPoint);
	//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����

	void PHeadLengthReset();
	//�`�F�[���̒��������Z����
	void CurPHeadLengPlus(float addPow);
	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	Vector2 LaneChange_Up();
	Vector2 LaneChange_Down(LaneChangeType changeType);

	//���ݕt���G�t�F�N�g�𐶐�
	void CreateBiteEffect();
	//��ύX�G�t�F�N�g�𐶐�
	void CreateMetamorEffect();

//�v���C���[�̏�Ԃɉ������X�V
protected:
	void ToFallMode(bool isPlaySE);
	void ToShootMode(bool isPlaySE);
	void ToShootEndMode(bool isPlaySE);
	void ToBiteMode(bool isPlaySE);
	void ToSlipMode(bool isPlaySE);
	void ToResistMode(bool isPlaySE);
	void ToClearMode(bool isPlaySE);
	void ToDeadMode(bool isPlaySE);
	void ToBackShootMode(bool isPlaySE);

	void FallUpdate();
	void ShootUpdate();
	void ShootEndUpdate();
	void BiteUpdate();
	void SlipUpdate();
	void ResistUpdate();
	void ClearUpdate();
	void DeadUpdate();
	void BackShootUpdate();

protected:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	using PSwordPtr = std::shared_ptr<Player_Sword>;
	//�X�e�[�W�̍ő僌�[����
	int maxLaneSize_;

	int addscorelist_[3];

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
	//��`��e�X�g
	std::vector<NeckPiecePoint> neckDrawPoints;

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

	float slipCount_;

	Vector2 headPosAddVect_;

	//���藎����܂ł̎���
	float slipResistTime_;

	//���̈ʒu�����肷��,0�ŉE
	float headAngleSetter;
	//0=�؋� 1=���ˎ� 2=���ˏI�� 3=���ݕt�� 4=���藎��
	int playerMode_;
	float jumpShotPower_;

	//���[���ړ������t���[���Ŋ��ޑΏۂ����邩�𒲂ׂ�
	bool isNextLaneBite_;

	int laneAddNum_;

	int chainLockCoolTime_;
	
	bool isClearShoot_;
	//�v���C���[�p�̃L�[���̓`�F�b�J�[
	PlayerInputChecker isUseKey_;
	bool isTutorialText_;
	//���ݒ͂�ł��镞
	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;
	//���鎞�Ԃ̔{��(����)
	std::map<CLOTHES_ID, float> slipCountMult_;
	//�X�V�֐����X�g
	std::map<int, std::function<void()>> updateFunctionMap_;

	//�N���A���Ƀv���C���[�����˂����u�Ԃ̏������s��
	MethodTimer clearShootTimer_;
	
	//������b�N���鏈�����s��
	bool addLengthLocker_;
protected:
	const float gravity_{0.5f};
	const float spdLimit{ 2.75f };

};

