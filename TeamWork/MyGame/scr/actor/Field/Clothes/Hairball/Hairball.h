#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

class Hairball : public Clothes, public std::enable_shared_from_this<Hairball>
{
public:
	//�R���X�g���N�^
	Hairball(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	virtual ~Hairball() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	Player_Head* player_Head_;
};