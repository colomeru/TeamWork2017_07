#pragma once

#include "../Actor.h"

class Efect : public Actor
{
public:
	// �R���X�g���N�^
	Efect(IWorld* world);
	// �f�X�g���N�^
	~Efect();
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

