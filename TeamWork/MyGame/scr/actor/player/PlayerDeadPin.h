#pragma once
#include "../Actor.h"

class PlayerDeadPin : public Actor, public std::enable_shared_from_this<PlayerDeadPin>
{
public:
	//�R���X�g���N�^
	PlayerDeadPin(IWorld* world, const Vector2& position);
	//�f�X�g���N�^
	~PlayerDeadPin();
	virtual void FastUpdate()override {
		laneNum_ = world_->GetKeepDatas().playerLane_;
		isUpdate_ = true;
		isDraw_ = true;
	}
	//�X�V
	virtual void Update() override;
	virtual void LateUpdate()override {
		isDraw_ = true;

	}
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
	const Vector2 addgravity{0.f,0.1f};
};