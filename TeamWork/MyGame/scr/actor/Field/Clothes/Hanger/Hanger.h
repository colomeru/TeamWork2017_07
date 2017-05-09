#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

//�n���K�[
class Hanger : public Clothes, public std::enable_shared_from_this<Hanger>
{
public:
	//�R���X�g���N�^
	Hanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~Hanger();
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
	int cnt_;
};