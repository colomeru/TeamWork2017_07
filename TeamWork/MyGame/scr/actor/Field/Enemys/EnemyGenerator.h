#pragma once
#include"../../Actor.h"

class EnemyGenerator :public Actor{
public:
	EnemyGenerator(IWorld* world);
	~EnemyGenerator();
	void FastUpdate()override {
		isUpdate_ = true;
	}
	virtual void Update()override;

	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:

	int timeCount_;
};