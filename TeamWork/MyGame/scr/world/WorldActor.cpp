#include "WorldActor.h"
#include <algorithm>

WorldActor::WorldActor()
{
	Initialize();
}

WorldActor::~WorldActor()
{
}

void WorldActor::Initialize()
{
	managers_[ACTOR_ID::BACK_GROUND_ACTOR] = ActorManager();
	managers_[ACTOR_ID::LANE_ACTOR] = ActorManager();
	managers_[ACTOR_ID::STAGE_ACTOR] = ActorManager();
	managers_[ACTOR_ID::HANGER_ACTOR] = ActorManager();
	managers_[ACTOR_ID::PLAYER_HEAD_ACTOR] = ActorManager();
	managers_[ACTOR_ID::PLAYER_ACTOR] = ActorManager();
	managers_[ACTOR_ID::UI_ACTOR] = ActorManager();

}

// �X�V
bool WorldActor::Update()
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

	return false;
}

bool WorldActor::ChangeLaneUpdate()
{
	bool isCLaneEnd_=false;
	EachActor([&](ActorManager& manager) { isCLaneEnd_=manager.CamMoveUpdate(); });

	return isCLaneEnd_;
}

bool WorldActor::StartModeUpdate()
{
	EachActor([&](ActorManager& manager) { manager.StartModeUpdate(); });
	return false;
}

// �`��
void WorldActor::Draw(const int laneCount,const int playerLane) const
{
	// �S�L�����`��
	//EachActor([&](const ActorManager& manager) { manager.Draw(laneCount,playerLane); });
	
	managers_.at(ACTOR_ID::BACK_GROUND_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::LANE_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::STAGE_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::HANGER_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::PLAYER_HEAD_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::PLAYER_ACTOR).Draw(laneCount, playerLane);
	managers_.at(ACTOR_ID::UI_ACTOR).Draw(laneCount, playerLane);
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
	Initialize();
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
