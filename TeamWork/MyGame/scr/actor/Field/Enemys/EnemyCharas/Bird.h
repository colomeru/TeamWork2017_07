#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"
#include"CharacterAnmManager.h"

class Bird : public Enemys, public std::enable_shared_from_this<Bird>
{
public:
	//�R���X�g���N�^
	Bird(IWorld* world, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~Bird();
	virtual void FastUpdate()override {
		isUpdate_ = true;
	}
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
	float timeCount_;
	int dropTimer_;
	Vector2 basePos_;

	CharacterAnmManager anmManager_;
};