#include "ActorManager.h"
#include "Actor.h"
#include <algorithm>

// �X�V
void ActorManager::Update()
{
	for (auto& actor : actorPtr)
	{
		actor->FastUpdate();
		actor->CommonUpdate();
		actor->Update();
		actor->LateUpdate();
		actor->LateComUpdate();
	}
}

// �`��
void ActorManager::Draw(const int laneCount, const int playerLane) const
{
	// ���[���������`��
	for (int i = 0; i <= laneCount; i++)
	{
		// �v���C���[�Ƃ̃��[������1�ȏ�Ȃ�ȗ�
		auto dif = playerLane - i;
		if (MathHelper::Abs(dif) > 1)
			continue;

		for (auto& actor : actorPtr)
		{
			auto thisLane = actor->GetLaneNum();
			// �`�惌�[���ƃA�N�^�[�̃��[���������Ă�����`��
			if (thisLane == i)
				actor->Draw();

			// �`���Actor���Œl�����߂�悤�ɂ���
			//actor->BaseDraw();
		}
	}
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
