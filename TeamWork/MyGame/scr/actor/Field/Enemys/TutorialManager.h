#pragma once
#include"../../Actor.h"
#include"../../../stageGenerator/Stage.h"

class TutorialManager :public Actor {
public:
	TutorialManager(IWorld* world, int stage);
	~TutorialManager();
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

	void StartTapperResurrectTimer();
private:

	int timeCount_;

	int windCount_;

	float resurTimer_;
	bool isResur_;


	int stage_;
};