#pragma once

#include"../../../actor/Actor.h"
#include<vector>

class StartCameraPointAct : public Actor//, public std::enable_shared_from_this<Player>
{
public:
	//�R���X�g���N�^
	StartCameraPointAct(IWorld* world,float startPosx);
	//�f�X�g���N�^
	~StartCameraPointAct();
	//�X�V
	virtual void FastUpdate() override {
		isUpdate_ = true;
	}
	virtual void Update() override;
	virtual void StartOnlyUpdate()override;
		//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

	bool isArrivePoint() const{
		return position_.x <= 0;
	}
private:

};