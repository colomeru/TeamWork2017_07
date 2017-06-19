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
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	void StartTapperResurrectTimer();
private:

	int timeCount_;

	int windCount_;

	float resurTimer_;
	bool isResur_;


	int stage_;
};