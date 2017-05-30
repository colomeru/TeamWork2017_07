#pragma once
#include "../MyGame/scr/actor/Actor.h"
#include "../MyGame/scr/actor/player/Player.h"

class HairballGenerator : public Actor
{
public:
	// �R���X�g���N�^
	HairballGenerator(IWorld* world, int lane_num, Vector2 pos);
	// �f�X�g���N�^
	~HairballGenerator();
	// �X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	// �󓮍X�V
	virtual void OnUpdate() override;
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;
	

private:
	Player* player_;

};