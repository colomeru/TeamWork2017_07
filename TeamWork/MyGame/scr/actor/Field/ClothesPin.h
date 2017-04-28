#pragma once
#include "../Actor.h"

class ClothesPin : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//�R���X�g���N�^
	ClothesPin(IWorld* world, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~ClothesPin();
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

	void ClearThis() {
		parameter_.isDead = true;
	}
private:
};