#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include "../../scene/Credit2Scene.h"

class Player_Head;
class Player_Sword;

//const Vector2 pHDist = Vector2(32, 32);
//const int headAngleSetter = -2;
//const float defHeadChangeTime = 0.2f;
//const float defHeadLength = 2.f;
//const float defPGravPow = 0.05f;
//const float defGravAddPow = 0.2f;
//const float HeadShootMult = 0.5f;
//const float defSlipCount = 8.f;
//const int defLaneChangeCoolTime_ = 60;
//const int defChainLockCoolTime_ = 10;
////���������position���Z�l
//const float fallAddPosMult = 3.4f;
////������܂ł̍ő厞��
//const float defResistTime = 0.5f;
//���[���̖{��

//enum {
//	MODE_FALL = 0,
//	MODE_SHOOT = 1,
//	MODE_SHOOT_END = 2,
//	MODE_BITE = 3,
//	MODE_SLIP = 4,
//	MODE_RESIST = 5,
//	MODE_CLEAR = 6,
//	MODE_PLAYERDEAD = 7
//};


class CreditPlayer : public Actor, public std::enable_shared_from_this<CreditPlayer>
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//�R���X�g���N�^
	CreditPlayer(IWorld* world, int maxLaneSize = 3, int startLane = 1);
	//�f�X�g���N�^
	~CreditPlayer();
	//�X�V
	virtual void Update() override;
	//�ŏ��̍X�V
	virtual void FastUpdate() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	//
	virtual bool CamMoveUpdate();
	//
	virtual void CamMoveUp()override;
	//
	virtual void CamMoveDown() override;
	//
	virtual void LaneChangeFall() override;
	//
	bool isLaneChangeFall() const;
	//�U��q�^��
	void Pendulum(Vector2 fulcrum, float length);
	//�X�^�[�g���̐U��q
	void StartPendulum();
	//���̍��W���擾
	Vector2 GetHeadPos(int headNum)const;
	//Head�̒��������ۂ̃Q�[���ɔ��f�����l�ɕϊ����ĕԂ�
	float GetHeadLengthChangeToPosMult(int headNum) const;
	//���݂�Head�̎�̒�����Ԃ�
	Vector2 GetCurrentHeadLength()const;
	//
	void HeadPosUpdate();
	//
	void SwordPosUpdate();
	//�g�p���铪���E�ׂ̕��ɕύX
	void changeHead();
	//�g�p���铪�����ׂ̕��ɕύX
	void backChangeHead();
	//�v���C���[�̊J�n���̏�Ԑݒ�
	void StartPlayerSet();
	//���݂̓��̔ԍ����擾
	int GetCurHead()const;
	//�����S
	bool GetPHeadDead(int pHeadNum)const;
	//����
	void CurHeadBite(const Vector2& target);
	//
	void ResurrectHead();
	//
	void SetOtherClothesID_(CLOTHES_ID cId);
	//���ݕt�����Ԃ���Ԃ�(���W�X�g�܂�)
	bool GetIsShootMode()const;
	//
	bool GetIsBiteMode()const;
	//
	bool GetIsResistMode()const;
	//
	bool GetIsClearMode()const;
	//
	int GetPHeadLiveCount()const;
	//���ݕt����Ԃɂ��邩���Z�b�g�A
	void SetIsBiteMode(bool ismode);
	//
	void SetMode(int pMode);
	//�V���[�g�I���̏u�Ԃ��ǂ��������
	bool GetIsShootModeEnd()const;
	//
	float GetSlipCount()const;
	//
	bool GetIsSlipped()const;
	//Head�̃��[����{�̂̃��[���ɍ��킹��
	void SetMyHeadLaneNum(int targetNum);
	//
	void SetAllHeadLaneNum();
	//world�̋��L�f�[�^�Ɏ����̏���������
	void worldSetMyDatas();
	//
	void SetNextLane(int addNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//
	void setCurPHeadSPos(const Vector2& sPos);
	//
	void setMaxLaneSize(int size);
	//���݂̓��ɑ΂��Ċ��邩�ǂ������Z�b�g����
	void curPHeadSlip(bool isSlip);
	//�v���C���[������ł邩
	bool isPlayerDead()const;
	//
	void PHeadChanger(int rot = 0);
	//
	void SetStopPos(Vector2 target);
	//
	Vector2 GetStopPos()const;
	//
	float GetRot()const;
	//
	Vector2 GetHeadPosAddVect() const;
	//
	Vector2 GetSlipHeadPoint()const;
	//���ݎg�p���Ă���Head�̍��W��Ԃ�
	Vector2 GetCurrentPHeadPosition()const;
	//
	float GetPlayerSwordAngle()const;
	//�U��q�̎x�_�ʒu���ړ�����
	void SetMultipleFulcrumPos(const Vector2& pos);
	//
	bool GetIsSwordActive()const;
	//�N���A��Ɏ��X�e�[�W�ɔ�ԏ�Ԃ��𒲂ׂ�
	bool GetIsClearBite()const;
	//
	bool GetIsClearShoot()const;
	//
	Actor* GetCurrentHead() const;
	//
	void deadLine();
	//�d�̓��Z�b�g
	void GravityReset();
	//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����
	void PHeadLengthReset();
	//���S��������S�ĉ�
	void AllResurrectHead();
	//
	void SetBiteSprite();
	//
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	//
	void DeformationDraw();
	//
	void SetPosition(Vector2 position);
	//
	Vector2 GetPlayerPosition();


private:
	//
	void Multiple();
	//���d�U��q�Ɉړ��ʂ����Z
	void UpdateMultiplePos();
	void SetMultiplePos(const Vector2& addpos);
	//���d�U��q�������I�Ɉړ�
	void AddMultiplePos(const Vector2& addPos);
	//
	void SetNeckNonMult();
	//��̕`��ɕK�v�Ȉ�A�̓�����s��
	void SetDrawNeck(const Vector2& bodyPoint, const Vector2& headPoint);
	//��̕`��ʒu��ݒ�
	void SetDrawPoint(const Vector2& bodyPoint, const Vector2& headPoint);
	//��̊e�`��ʒu��ݒ肷��
	void SetDrawNeckParts(const Vector2& bodyPoint, const Vector2& headPoint);
	//���͂ɂ�铮����܂Ƃ߂�
	void PlayerInputControl();
	//�`�F�[���̒��������Z����
	void CurPHeadLengPlus(float addPow);
	//
	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//
	void CreateBiteEffect();


	//�v���C���[�̏�Ԃɉ������X�V
private:
	//
	void FallUpdate();
	//
	void ShootUpdate();
	//
	void ShootEndUpdate();
	//
	void BiteUpdate();
	//
	void SlipUpdate();
	//
	void ResistUpdate();
	//
	void ClearUpdate();
	//
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
	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;

	//���鎞�Ԃ̔{��(����)
	std::map<CLOTHES_ID, float> slipCountMult_;

	std::map<int, std::function<void()>> updateFunctionMap_;

	bool operatre_;

};

