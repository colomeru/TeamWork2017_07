#pragma once
#include "../Actor.h"

class PlayerFallPin : public Actor, public std::enable_shared_from_this<PlayerFallPin>
{
public:
	//�R���X�g���N�^
	PlayerFallPin(IWorld* world, const Vector2& position,const Vector2& toPos);
	//�f�X�g���N�^
	~PlayerFallPin();
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

private:
	int sinCount_;
	Vector2 direction_;
	Vector2 gravity_;
	const Vector2 addgravity{ 0.f,0.1f };
};