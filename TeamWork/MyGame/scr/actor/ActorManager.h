#pragma once
#include "ActorPtr.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include <functional>
#include <list>

class ActorManager
{
public:
	// �R���X�g���N�^
	ActorManager() = default;
	// �f�X�g���N�^
	~ActorManager() = default;
	// �X�V
	void Update();
	bool CamMoveUpdate();
	bool StartModeUpdate();
	// �`��
	void Draw(const int laneCount=0, const int playerLane=0) const;
	// �ǉ�
	void Add(ActorPtr actor_);
	// �폜
	void Clear();
	// �폜
	void Remove();
	// �Փ˔���
	void Collide(COL_ID id, Actor& other);
	// �Փ˔���
	void Collide(COL_ID id, ActorManager& other);

	// �q������
	void EachActor(std::function<void(Actor&)> func);
	// �q������iconst�Łj
	void EachActor(std::function<void(const Actor&)> func) const;
	// �A�N�^�[����Ԃ�
	int Count() const;
	// ���b�Z�[�W����
	void HandleMessage(EventMessage message, void* param);
	// ���X�g�̎擾
	std::list<ActorPtr> GetList() const;

	void DrawUpdate();
private:
	std::list<ActorPtr>	actorPtr;
};