#pragma once
#include "../Actor.h"

class Enemy : public Actor
{
public:
	// �R���X�g���N�^
	Enemy(IWorld* world);
	// �f�X�g���N�^
	~Enemy();
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
	bool isHit_;
};

