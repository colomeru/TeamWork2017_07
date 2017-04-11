#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include <stack>

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

private:
	// �A�N�^�[
	WorldActor	actors_;

	// �󓮍X�V�A�N�^�[�p�X�^�b�N
	std::stack<ActorPtr>	manualStackActor_;
	// �J�����p�X�^�b�N
	std::stack<ActorPtr>	stackCamera_;
};