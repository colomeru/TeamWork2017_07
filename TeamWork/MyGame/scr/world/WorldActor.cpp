#include "WorldActor.h"
#include <algorithm>

WorldActor::WorldActor()
{
}

WorldActor::~WorldActor()
{
}

// �X�V
void WorldActor::Update()
{
	// �S�L�����A�b�v�f�[�g
	EachActor([&](ActorManager& manager) { manager.Update(); });

	// �Փ˔���
	for (auto& cols : colselect_)
		for (auto& sec : cols.second)
			managers_[sec.otherID].Collide(sec.colID, *cols.first);

	// �����蔻��N���A
	colselect_.clear();

	// ���S�L�����폜
	EachActor([](ActorManager& manager) { manager.Remove(); });
}

// �`��
void WorldActor::Draw(const int laneCount, const int playerLane) const
{
	// �S�L�����`��
	EachActor([&](const ActorManager& manager) { manager.Draw(laneCount, playerLane); });
}

// �ǉ�
void WorldActor::Add(ACTOR_ID id, ActorPtr actor)
{
	managers_[id].Add(actor);
}

// �N���A
void WorldActor::Clear()
{
	// �S�L�����N���A
	managers_.clear();
	// �����蔻��N���A
	colselect_.clear();
}

// �Փ˔���I��
void WorldActor::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	CollideSelect c = { otherID, colID };
	colselect_[thisActor].push_back(c);
}

// �w��O���[�v�̏���
void WorldActor::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	auto manager = managers_.find(id);
	if (manager == managers_.end()) return;
	manager->second.EachActor(func);
}

// �w��O���[�v�̏���
void WorldActor::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	const auto manager = managers_.find(id);
	if (manager == managers_.cend()) return;
	manager->second.EachActor(func);
}

// �w��O���[�v�̃A�N�^�[�����擾
int WorldActor::Count(ACTOR_ID id) const
{
	auto manager = managers_.find(id);
	if (manager == managers_.end()) return 0;
	return manager->second.Count();
}

// ���b�Z�[�W�̏���
void WorldActor::HandleMessage(EventMessage message, void * param)
{
	// ���[���h�̃��b�Z�[�W����
	for (auto& lis : lis_)
		lis(message, param);

	// �A�N�^�[�̃��b�Z�[�W����
	for (auto& pair : managers_)
		pair.second.HandleMessage(message, param);
}

// �C�x���g���X�i�[�̒ǉ�
void WorldActor::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	lis_.push_back(listener);
}

// �}�l�[�W���[������
void WorldActor::EachActor(std::function<void(ActorManager&)> func)
{
	std::for_each(managers_.begin(), managers_.end(),
		[&](std::pair<const ACTOR_ID, ActorManager>& e)
	{
		func(e.second);
	});
}

// �}�l�[�W���[������iconst�Łj
void WorldActor::EachActor(std::function<void(const ActorManager&)> func) const
{
	std::for_each(managers_.cbegin(), managers_.cend(),
		[&](const std::pair<const ACTOR_ID, ActorManager>& e)
	{
		func(e.second);
	});
}
