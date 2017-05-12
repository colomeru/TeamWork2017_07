#pragma once
#include "../MyGame/scr/actor/Actor.h"

class HairballGenerator : public Actor
{
public:
	// �R���X�g���N�^
	HairballGenerator(IWorld* world, int lane_num);
	// �f�X�g���N�^
	~HairballGenerator();
	// �X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	Actor* player_;

};