#include "World.h"
#include "../actor/Actor.h"

// �R���X�g���N�^
World::World()
{
}

// �f�X�g���N�^
World::~World()
{
	Clear();
}

// ������
void World::Initialize()
{
	Clear();
}

// �X�V
void World::Update()
{
	actors_.Update();
	
	// �󓮍X�V
	if (!manualStackActor_.empty())
		manualStackActor_.top()->OnUpdate();

	// �J�����X�V
	//if (!stackCamera_.empty())
	//	stackCamera_.top()->OnUpdate();
}

// �`��
void World::Draw() const
{
	actors_.Draw();
}

// �N���A
void World::Clear()
{
	// �X�^�b�N����
	while (!manualStackActor_.empty())
		manualStackActor_.pop();
	while (!stackCamera_.empty())
		stackCamera_.pop();

	// �A�N�^�[����
	actors_.Clear();
}

// �C�x���g���X�i�[�̒ǉ�
void World::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	actors_.AddEventMessageListener(listener);
}

// �A�N�^�[�̎擾
WorldActor World::GetWorldActor() const
{
	return actors_;
}

// �ǉ�
void World::Add(ACTOR_ID id, ActorPtr actor)
{
	actors_.Add(id, actor);
}

// 
bool World::IsEnd() const
{
	return false;
}

// �Փ˔���
void World::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	actors_.SetCollideSelect(thisActor, otherID, colID);
}

// ���b�Z�[�W�̑��M
void World::sendMessage(EventMessage message, void * param)
{
	actors_.HandleMessage(message, param);
}

// �w��O���[�v�̏���
void World::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	actors_.EachActor(id, func);
}

// �w��O���[�v�̏���
void World::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	actors_.EachActor(id, func);
}

int World::Count(ACTOR_ID id) const
{
	return actors_.Count(id);
}

// ����A�N�^�[�Z�b�g
void World::PushStackActor(ActorPtr actor)
{
	manualStackActor_.push(actor);
}

// ����A�N�^�[�|�b�v
void World::PopStackActor()
{
	if (!manualStackActor_.empty())
		manualStackActor_.pop();
}