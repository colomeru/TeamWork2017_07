#pragma once
#include "../Actor.h"
#include "../../math/Vector2.h"

class Player3 : public Actor, public std::enable_shared_from_this<Player3>
{
public:
	//�R���X�g���N�^
	Player3(IWorld* world);
	//�f�X�g���N�^
	~Player3();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

	//�U��q�^��
	void Pendulum(Vector2 fulcrum, float length);

private:
	//��]�p�x
	//�Փ˂��Ă��邩
	bool isHit_;
	float jumpVec;

	//�U��q�֘A
	//�x�_���W
	Vector2 fulcrum_;
	//�p�x
	float rot_;
	//�p���x
	float rot_spd_;
	//�R�̒���
	float length_;
	//�d�͉����x
	float gravity_;
};