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
#include"../Def.h"

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
	virtual ~Actor();
	// �X�V
	virtual void Update() = 0;
	// �`��
	void BaseDraw()const {
		if (!isDraw_)return;
		else
		{
			int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
			drawLane = MathHelper::Abs(drawLane);
			Draw();

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
	Vector2 GetDrawAddPos()const {
		return drawAddPos_;
	}
	void FastComUpdate() {
		drawAddPos_ = Vector2::Zero;
		if (world_->isChangeFrame()) { 
			isUpdate_ = true;
		}
		else {
			isUpdate_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
		}
	}
	virtual void LateUpdate() {

	}
	void CommonUpdate() {
		prevPosition_ = position_;
	}
	void LateComUpdate();
	//���[���ړ�������̃A�b�v�f�[�g�Avirtual�����APlayer�ȊO��override���Ȃ��悤�ɂ��鎖
	virtual bool CamMoveUpdate() {
		CamMoveOnlyUpdate();
		parameter_.spriteAlpha_ = 0.5f;
		if (0 > world_->GetKeepDatas().nextLane_) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}
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
		isDraw_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
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
	Vector2 GetVelocity()const {
		return velocity_;
	}
	Vector2 GetDrawPosVect(const Vector2& pos)const{
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);
		
		int drawLane;
		drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		
		if (world_->GetIsFreeCamY_()&&drawLane < 0) {

			retPos.y += defDrawLinePosY[drawLane + 1]*1.2f;

			return retPos;
		}

		if (MathHelper::Abs (drawLane) >= 2) {
			retPos.y = -500;
		}
		else {
			retPos.y += defDrawLinePosY[drawLane + 1];

		}
		retPos += drawAddPos_;
		return retPos;
	}
	//lanenum�����R�Ɏw�肵��drawpos���Z�o
	Vector2 GetFreeActorDrawPos(const Vector2& pos,int laneNum) const{
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);

		int drawLane;
		drawLane = laneNum - world_->GetKeepDatas().playerLane_;

		if (world_->GetIsFreeCamY_() && drawLane < 0) {

			retPos.y += defDrawLinePosY[drawLane + 1] * 1.2f;

			return retPos;
		}

		if (MathHelper::Abs(drawLane) >= 2) {
			retPos.y = -500;
		}
		else {
			retPos.y += defDrawLinePosY[drawLane + 1];

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
	void DrawUpdate() {
		drawPos_ = GetDrawPosVect(position_);
	}
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
};