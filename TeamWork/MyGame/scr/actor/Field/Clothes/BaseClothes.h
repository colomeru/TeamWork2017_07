#pragma once
#include "Clothes.h"

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//�R���X�g���N�^
	BaseClothes(IWorld* world, CLOTHES_ID clothes, Vector2 pos);
	//�f�X�g���N�^
	~BaseClothes();
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

};