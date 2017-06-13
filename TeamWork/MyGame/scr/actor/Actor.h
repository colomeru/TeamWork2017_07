#pragma once
#include "ActorParameter.h"
#include "ActorPtr.h"
#include "../collision/CollisionFunction.h"
#include "../collision/CollisionParameter.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include "../math/Quaternion.h"
#include "../world/IWorld.h"
#include <functional>
#include <map>
#include"../math/Vector3.h"
#include"../world/World.h"
#include"../graphic/Sprite.h"

#include "DxLib.h"

static const float alphaSetter[2] = { 1.f,0.5f };
//�X�V�y�є���̐����͈́A0=�X�V�͈͂̍��[,1=�X�V�͈͂̉E�[,2=����͈͂̍��[,3=����͈͂̉E�[
static const int cutSize[4] = { 600,2200 ,400,800};
class Actor
{
public:
	// �R���X�g���N�^
	Actor(IWorld* world, Actor* parent = nullptr);
	// ���z�f�X�g���N�^
	virtual ~Actor() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	void BaseDraw()const {
		if (!isDraw_ || !isUpdate_)return;
		else
		{
			int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
			drawLane = MathHelper::Abs(drawLane);
			//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaSetter[drawLane]);
			Draw();
			//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		}
	}
	virtual void Draw() const = 0;
	// ���S���Ă��邩�H
	virtual bool isDead() const { return parameter_.isDead; }
	// ���S����
	void Dead() { parameter_.isDead = true; };

	// �Փ˔���
	void Collide(COL_ID id, Actor& other);
	// �p�����[�^�̎擾
	ActorParameter GetParameter() const;
	// �|�[�Y�̎擾
	Matrix GetPose() const;
	// �|�[�Y�̐ݒ�
	virtual void SetPose(const Matrix& mat);
	// �󓮍X�V
	virtual void OnUpdate();

	void UpdateList() {
		if (!isUpdate_)return;
		Update();
	}
	virtual void FastUpdate() {
	}
	void FastComUpdate() {
		drawAddPos_ = Vector2::Zero;
		if (world_->isChangeFrame()) { 
			isUpdate_ = true;
		}
		else {
			isUpdate_ = (world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[0] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[1]);
		}
	}
	virtual void LateUpdate() {

	}
	void CommonUpdate() {
		prevPosition_ = position_;
	}
	void LateComUpdate() {
		Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
		//drawPos_ = Vector2(cmpos3d.x, cmpos3d.y);
		//OutputDebugString(std::to_string(world_->GetKeepDatas().playerLane_).c_str());
		//OutputDebugString("\n");
		//OutputDebugString(std::to_string(world_->GetKeepDatas().playerLane_).c_str());
		//OutputDebugString("\n");
		drawPos_ = GetDrawPosVect(position_);

		int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		if (MathHelper::Abs(drawLane) >= 2) {
			isDraw_ = false;
		}
		else {
			isDraw_ = true;
			//drawPos_.y += defDrawLinePosY[drawLane + 1];

			drawLane = MathHelper::Abs(drawLane);
			parameter_.spriteAlpha_ = alphaSetter[drawLane];
		}

		isCheckCol_ =	(world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[2] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[3])
						&&laneNum_ == world_->GetKeepDatas().playerLane_;
		if (world_->isChangeFrame()) {

		}
	}
	//���[���ړ�������̃A�b�v�f�[�g�Avirtual�����APlayer�ȊO��override���Ȃ��悤�ɂ��鎖
	virtual bool CamMoveUpdate() {
		CamMoveOnlyUpdate();
		parameter_.spriteAlpha_ = 0.5f;
		laneChangeFunctionMap_[world_->GetKeepDatas().nextLane_]();
		drawPos_ = GetDrawPosVect(position_);
		return true;
	}
	virtual void CamMoveOnlyUpdate(){}
	virtual void StartOnlyUpdate() {

	}
	virtual void StartOnlyLateUpdate() {

	}
	bool StartModeUpdate() {
		StartOnlyUpdate();
		isDraw_ = (world_->GetKeepDatas().startPointPos_.x - position_.x < cutSize[0] && position_.x - world_->GetKeepDatas().startPointPos_.x < cutSize[1]);
		isUpdate_ = true;

		StartOnlyLateUpdate();
		return false;
	}
	virtual void CamMoveUp() {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_-laneNum_+2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum+1], laneLerpNum)- defDrawLineChangePosY[targetNum];
	}
	virtual void CamMoveDown() {
		LaneChangeFall();
	}
	Vector2 GetDrawPosVect(const Vector2& pos)const{
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);
		
		int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		if (MathHelper::Abs (drawLane) >= 2) {
			//isDraw_ = false;
			retPos.y = -500;
		}
		else {
			//isDraw_ = true;
			retPos.y += defDrawLinePosY[drawLane + 1];

			//drawLane = MathHelper::Abs(drawLane);
			//spriteAlpha_ = alphaSetter[drawLane];
		}
		retPos += drawAddPos_;
		return retPos;
	}
	// �����擾
	Actor* GetActor() const;
	// �e�擾
	Actor* GetParent() const;

	Vector2 GetPosition() const {

		return position_;
	}
	Vector2 GetDrawPos()const {
		return drawPos_;
	}
	Vector2 GetPrevPosition() const {
		return prevPosition_;
	}
	float GetAngle() const {
		return angle_;
	}
	int GetLaneNum() const {
		return laneNum_;
	}
	bool GetIsUpdate()const {
		return isUpdate_;
	}
	bool GetIsDraw()const {
		return isDraw_;
	}
	void SetLaneNum(int laneNum) {
		laneNum_ = laneNum;
	}
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

protected:
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param);
	//�����鎞�̃��[���ύX
	virtual void LaneChangeFall() {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
	}
private:
	//CollisionParameter Test_Col(const Actor& other) const;

	//// �X�v���C�g�̓����蔻��i�Q������OBB vs OBB�j
	//CollisionParameter IsHit_OBB_OBB(const Actor& sprite2);
	//CollisionParameter IsHit_OBB_Segment(const Actor& sprite2);
	//CollisionParameter IsHit_Segment_Segment(const Actor& sprite2);
	//CollisionParameter IsHit_Circle_Circle(const Actor& sprite2);
	//CollisionParameter IsHit_Circle_Segment(const Actor& sprite2);
	//CollisionParameter IsHit_OBB_Circle(const Actor& sprite2);
	//CollisionParameter IsHit_OBB_Clothes(const Actor & sprite2);
	//CollisionParameter IsHit_Clothes_OBB(const Actor & sprite2);

	/* �R�s�[�֎~ */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//�X�V�A����A�`����s�����ǂ���
	bool isUpdate_;
	bool isCheckCol_;
	//���g���`�悷��摜��ID
	SPRITE_ID spriteId_;
	// ���[���h
	IWorld*				world_;
	// �R���W�����t�@���N�V����
	CollisionFunction	colFunc_;
	// �e
	Actor*				parent_;

	// �p�����[�^
	ActorParameter	parameter_;
	// �ʒu
	Vector2			position_;
	//�O�t���[���̈ʒu
	Vector2			prevPosition_;

	//�e�A�N�^�[�̕`��ʒu���v�Z�����l
	Vector2			drawPos_;
	//���[���ύX����drawpos�ɉ��Z����l
	Vector2			drawAddPos_;
	bool isDraw_;
	// �ړ���
	Vector2			velocity_;
	//���g���������Ă��郌�[���̔ԍ�
	int				laneNum_;
	//���g�̊p�x
	float			angle_;
	
	// �t�@���N�V�����}�b�v
	std::map<COL_ID, std::function<CollisionParameter(const Actor&, const Actor&)>> colFuncMap_;
	std::map<int, std::function<void()>> laneChangeFunctionMap_;
	//std::map<COL_ID, std::function<CollisionParameter(const Actor& sprite2)>> colFuncMap_;
};