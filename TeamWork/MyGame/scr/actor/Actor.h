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
static const int cutSize[2] = { 500,1800 };
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
	void FastUpdate() {
		isUpdate_ = (world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[0] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[1]);
	}
	virtual void LateUpdate() {

	}
	void CommonUpdate() {
		prevPosition_ = position_;
	}
	void LateComUpdate() {
		Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
		drawPos_ = Vector2(cmpos3d.x, cmpos3d.y);


		int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		if (drawLane >= 2) {
			isDraw_ = false;
		}
		else {
			isDraw_ = true;
			drawPos_.y += defDrawLinePosY[drawLane + 1];

			drawLane = MathHelper::Abs(drawLane);
			spriteAlpha_ = alphaSetter[drawLane];
		}
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
	SPRITE_ID spriteId_;
	float spriteAlpha_;
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
	Vector2			prevPosition_;

	Vector2			drawPos_;
	bool isDraw_;
	// �ړ���
	Vector2			velocity_;

	int				laneNum_;

	float			angle_;

	// �t�@���N�V�����}�b�v
	std::map<COL_ID, std::function<CollisionParameter(const Actor&, const Actor&)>> colFuncMap_;
	//std::map<COL_ID, std::function<CollisionParameter(const Actor& sprite2)>> colFuncMap_;
};