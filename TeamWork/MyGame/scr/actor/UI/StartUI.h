#pragma once
#include "../Actor.h"
#include"../../math/Vector2.h"

class StartUI : public Actor
{
public:
	// �R���X�g���N�^
	StartUI(IWorld* world, Vector2 pos);
	// �f�X�g���N�^
	~StartUI();
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
	int timeCount_;
	Vector2 txtSize_;
};

