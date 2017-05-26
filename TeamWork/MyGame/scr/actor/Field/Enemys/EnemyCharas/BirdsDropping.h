#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"

class BirdsDropping : public Enemys, public std::enable_shared_from_this<BirdsDropping>
{
public:
	//�R���X�g���N�^
	BirdsDropping(IWorld* world, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~BirdsDropping();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
	Player* player_;
	Actor* player_Head_;
	Vector2 basePos_;

};