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
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	void StartTapperResurrectTimer();
private:

	int timeCount_;

	float resurTimer_;
	bool isResur_;
};