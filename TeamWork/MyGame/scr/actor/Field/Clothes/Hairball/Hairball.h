#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"

class Hairball : public Clothes, public std::enable_shared_from_this<Hairball>
{
public:
	//�R���X�g���N�^
	Hairball(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~Hairball();
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
};