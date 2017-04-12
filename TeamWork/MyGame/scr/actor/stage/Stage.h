#pragma once

#include "../Actor.h"

class Stage : public Actor
{
public:
	//�R���X�g���N�^
	Stage(IWorld* world);
	//�f�X�g���N�^
	~Stage();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

};