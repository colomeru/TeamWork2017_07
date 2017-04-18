#pragma once

#include "../actor/Actor.h"
#include "../math/Vector2.h"
class PlayerLine : public Actor, public std::enable_shared_from_this<PlayerLine>
{
public:
	//�R���X�g���N�^
	PlayerLine(IWorld* world);
	//�f�X�g���N�^
	~PlayerLine();
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
	float angle_;
	//�Փ˂��Ă��邩
	bool isHit_;
	
	Vector2 line1_;
	Vector2 head1_;
	
	Vector2 line2_;
	Vector2 head2_;

	Vector2 pos_;
};