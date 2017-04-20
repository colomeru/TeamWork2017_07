#include "ActorManager.h"
#include "Actor.h"
#include <algorithm>

// �X�V
void ActorManager::Update()
{
	for (auto& actor : actorPtr)
	{
		actor->CommonUpdate();
		actor->Update();
		actor->LateComUpdate();
	}
}

// �`��
void ActorManager::Draw() const
{
	for (auto& actor : actorPtr)
		actor->Draw();
}

// �ǉ�
void ActorManager::Add(ActorPtr actor_)
{
	actorPtr.push_back(actor_);
}

// �N���A
void ActorManager::Clear()
{
	actorPtr.clear();
}

// �폜
void ActorManager::Remove()
{
	actorPtr.remove_if([](ActorPtr actor)
	{
		return actor->isDead();
	});
}

// �Փ˔���
void ActorManager::Collide(COL_ID id, Actor & other)
{
	for (auto& actor : actorPtr)
		other.Collide(id, *actor);
}

// �Փ˔���
void ActorManager::Collide(COL_ID id, ActorManager & other)
{
	for (auto& actor : actorPtr)
		other.Collide(id, *actor);
}

// �q������
void ActorManager::EachActor(std::function<void(Actor&)> func)
{
	for (auto& actor : actorPtr)
		func(*actor);
}

// �q������iconst�Łj
void ActorManager::EachActor(std::function<void(const Actor&)> func) const
{
	for (auto& actor: actorPtr)
		func(*actor);
}

// �A�N�^�[����Ԃ�
int ActorManager::Count() const
{
	return (int)actorPtr.size();
}

// ���b�Z�[�W����
void ActorManager::HandleMessage(EventMessage message, void * param)
{
	EachActor([&](Actor& other)
	{
		other.handleMessage(message, param); 
	});
}

std::list<ActorPtr> ActorManager::GetList() const
{
	return actorPtr;
}
