#pragma once
#include "../Actor.h"

class Player2 : public Actor, public std::enable_shared_from_this<Player2>
{
public:
	//�R���X�g���N�^
	Player2(IWorld* world);
	//�f�X�g���N�^
	~Player2();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
	//��]�p�x
	//�Փ˂��Ă��邩
	bool isHit_;
	float jumpVec;
};