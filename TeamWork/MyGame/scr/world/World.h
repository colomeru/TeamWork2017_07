#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include"../math/MathHelper.h"
#include"../math/Vector3.h"
#include <stack>

static const Vector2 playerScreenPos_ = Vector2(300, 600);
class World : public IWorld
{
public:
	// �R���X�g���N�^
	World();
	// �f�X�g���N�^
	~World();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw() const;
	// �N���A
	void Clear();
	// �C�x���g���X�i�[�̒ǉ�
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

	// �A�N�^�[�̎擾
	WorldActor GetWorldActor() const;

	/* ���[���h�C���^�[�t�F�[�X */
	// �ǉ�
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	virtual void SetTarget(Actor* tgt){
		targetAct_ = tgt;
	}
	// �I����Ă��邩�H
	virtual bool IsEnd() const;
	// �Փ˔���ݒ�
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);
	// ���b�Z�[�W�̑��M
	virtual void sendMessage(EventMessage message, void* param = nullptr) override;
	// �w��O���[�v�̏���
	virtual void EachActor(ACTOR_ID id, std::function<void(Actor&)> func) override;
	// �w��O���[�v�̏���
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const override;
	// �w��O���[�v�̃A�N�^�[�����擾
	virtual int Count(ACTOR_ID id) const override;

	// ����A�N�^�[�Z�b�g
	virtual void PushStackActor(ActorPtr actor);
	// ����A�N�^�[�|�b�v
	virtual void PopStackActor();

	virtual void inv(const Matrix& mat) override;
	virtual Matrix InitializeInv(Vector2 position) override;
	
	virtual Matrix GetInv()override {
		return inv_;
	}
	virtual void SetScroolPos(const Vector2& pos) override {
		targetMat_.Translation(Vector3(pos.x, pos.y, 0));
	}

private:
	void Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f) const
	{
		// �o�l�̐L�ы���v�Z
		Vector2 stretch = (pos - resPos);
		// �o�l�̗͂��v�Z
		Vector2 force = -stiffness * stretch;
		// �����x��ǉ�
		Vector2 acceleration = force / mass;
		// �ړ����x���v�Z
		velo = friction * (velo + acceleration);

		pos = pos + velo;
	}

private:
	
	Matrix inv_;
	Matrix resInv_;
	Matrix targetMat_;
	Actor* targetAct_;
	Vector2 mPrePos;
	Vector2 mCurPos;
	// �A�N�^�[
	WorldActor	actors_;

	Vector2 mVelo;
	Vector2 velo;

	// �󓮍X�V�A�N�^�[�p�X�^�b�N
	std::stack<ActorPtr>	manualStackActor_;
	// �J�����p�X�^�b�N
	std::stack<ActorPtr>	stackCamera_;
};