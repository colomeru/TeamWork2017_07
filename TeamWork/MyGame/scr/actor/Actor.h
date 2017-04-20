#pragma once
#include "ActorParameter.h"
#include "ActorPtr.h"
#include "../collision/CollisionParameter.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include "../math/Quaternion.h"
#include "../world/IWorld.h"
#include <functional>
#include <map>
#include"../math/Vector3.h"

#include "DxLib.h"

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
	void CommonUpdate() {
		prevPosition_ = position_;
	}

	void LateComUpdate() {
		Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
		drawPos_ = Vector2(cmpos3d.x, cmpos3d.y);
	}

	// �����擾
	Actor* GetActor() const;
	// �e�擾
	Actor* GetParent() const;

	Vector2 GetPosition() const{

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
	void SetLaneNum(int laneNum){
		laneNum_= laneNum;
	}
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

protected:
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param);

private:
	CollisionParameter Test_Col(const Actor& other) const;

	// �X�v���C�g�̓����蔻��i�Q������OBB vs OBB�j
	CollisionParameter IsHit_OBB_OBB(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Segment(const Actor& sprite2);
	CollisionParameter IsHit_Segment_Segment(const Actor& sprite2);
	CollisionParameter IsHit_Circle_Circle(const Actor& sprite2);
	CollisionParameter IsHit_Circle_Segment(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Circle(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Clothes(const Actor & sprite2);

	/* �R�s�[�֎~ */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ���[���h
	IWorld*			world_;
	// �e
	Actor*			parent_;

	// �p�����[�^
	ActorParameter	parameter_;
	// �ʒu
	Vector2			position_;
	Vector2			prevPosition_;

	Vector2			drawPos_;
	// �ړ���
	Vector2			velocity_;

	int				laneNum_;

	float			angle_;
private:
	// �t�@���N�V�����}�b�v
	std::map<COL_ID, std::function<CollisionParameter(const Actor& sprite2)>> colFunc_;
};