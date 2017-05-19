#pragma once
#include "../game/ID.h"
#include "../actor/ActorManager.h"
#include "../actor/ActorPtr.h"
#include <map>
#include <vector>
#include <functional>


struct CollideSelect
{
	ACTOR_ID	otherID;
	COL_ID		colID;
};

class WorldActor
{
public:
	// �R���X�g���N�^
	WorldActor();
	// �f�X�g���N�^
	~WorldActor();
	// �X�V
	bool Update();
	bool ChangeLaneUpdate();
	bool StartModeUpdate();
	// �`��
	void Draw(const int laneCount=0,const int playerLane=0) const;
	// �ǉ�
	void Add(ACTOR_ID id, ActorPtr actor);
	// �N���A
	void Clear();
	// �Փ˔���
	void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);

	// �w��O���[�v�̏���
	void EachActor(ACTOR_ID id, std::function<void(Actor&)> func);
	// �w��O���[�v�̏���
	void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const;
	// �w��O���[�v�̃A�N�^�[�����擾
	int Count(ACTOR_ID id) const;

	// ���b�Z�[�W�̏���
	void HandleMessage(EventMessage message, void* param);
	// �C�x���g���X�i�[�̒ǉ�
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

private:
	// �}�l�[�W���[������
	void EachActor(std::function<void(ActorManager&)> func);
	// �}�l�[�W���[������iconst�Łj
	void EachActor(std::function<void(const ActorManager&)> func) const;



private:
	// �}�l�[�W���[�O���[�v
	std::map<ACTOR_ID, ActorManager>			   managers_;
	// �R���W�����O���[�v
	std::map<ActorPtr, std::vector<CollideSelect>> colselect_;

	// �C�x���g���X�i�[
	typedef std::function<void(EventMessage, void*)> Listener;
	std::vector<Listener> lis_;
};

