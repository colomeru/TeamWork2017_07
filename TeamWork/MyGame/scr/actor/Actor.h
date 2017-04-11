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

	// �����擾
	Actor* GetActor() const;
	// �e�擾
	Actor* GetParent() const;

	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

protected:
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param);

private:
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
	Vector3			position_;
	// �ړ���
	Vector3			velocity_;

private:
	// �t�@���N�V�����}�b�v
	std::map<COL_ID, std::function<CollisionParameter(const Actor&)>> colFunc_;
};