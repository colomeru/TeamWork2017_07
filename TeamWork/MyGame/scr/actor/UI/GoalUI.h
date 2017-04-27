#pragma once
#include "../Actor.h"

class GoalUI : public Actor
{
public:
	// �R���X�g���N�^
	GoalUI(IWorld* world, Vector2 pos);
	// �f�X�g���N�^
	~GoalUI();
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() const override;
	// �󓮍X�V
	virtual void OnUpdate() override;
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	
};

