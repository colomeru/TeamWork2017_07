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
	//�v���C���[������ł��Ȃ���Ԃł̓��Z�b�g���鏈��
	void Cancel();
	//�v���C���[����ɏオ���A�̏���
	void UpPlayer();

private:
	//�n���K�[�̋@�\�������Ă��邩
	bool isMove_;
	//���������Ă��邩�ǂ���
	bool isPull_;
	//�R�̒��S�̊�_
	Vector2 baseCenter_;
	//�v���C���[�̓��̍X�V���W
	Vector2 pHeadPos_;
	//�n���K�[�̂��Ȃ�̈ʒu
	Vector2 bonePos_[2];
	//�R�̕`��p�̈ʒu
	Vector2 codePos_[6];
	//�R�̒��S�_
	Vector2 codeCenterPos_;
};