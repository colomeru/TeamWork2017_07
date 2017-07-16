#pragma once
#include "../Clothes.h"

//�n���K�[
class UpHanger : public Clothes, public std::enable_shared_from_this<UpHanger>
{
public:
	//�R���X�g���N�^
	UpHanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	virtual ~UpHanger() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	//�΂˂̌v�Z
	void Spring(const Vector2& pos, float stiffnes, float friction, float mass);

private:
	bool isMove_;
	bool isPull_;
	Vector2 startPos_;
	Vector2 pHeadPos_;
	Vector2 codePos_[6];
	Vector2 codeCenterPos_;
};