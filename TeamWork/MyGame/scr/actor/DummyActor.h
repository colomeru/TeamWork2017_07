#pragma once
#include "Actor.h"

class DummyActor : public Actor
{
public:
	//�R���X�g���N�^
	explicit DummyActor(IWorld* world);
	//�f�X�g���N�^
	~DummyActor();
	//�X�V
	virtual void Update() override;
	//�ŏ��̍X�V
	virtual void FastUpdate() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	//
	virtual bool CamMoveUpdate();
	//
	virtual void CamMoveUp()override;
	//
	virtual void CamMoveDown() override;
	//
	virtual void LaneChangeFall() override;
};