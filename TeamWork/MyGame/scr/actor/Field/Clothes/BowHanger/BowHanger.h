#pragma once
#include "../Clothes.h"

//�n���K�[
class BowHanger : public Clothes, public std::enable_shared_from_this<BowHanger>
{
public:
	//�R���X�g���N�^
	BowHanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	virtual ~BowHanger() override;
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
	void Cancel();

private:
	bool isMove_;
	bool isPull_;
	Vector2 baseCenter_;
	Vector2 pHeadPos_;
	Vector2 codePos_[6];
	Vector2 codeCenterPos_;
};