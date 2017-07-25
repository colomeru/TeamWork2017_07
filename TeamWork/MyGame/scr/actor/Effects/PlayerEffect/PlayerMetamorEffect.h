#pragma once
#include"../Effect.h"

class PlayerMetamorEffect:public Effect{
public:
	PlayerMetamorEffect(IWorld* world, const Vector2& position,const std::shared_ptr<Actor>& target, float delay=0.0f, const Vector2& addVec=Vector2::Zero);

	void Update()override;
	void Draw()const override;
	void OnMessage(EventMessage message, void * param)override;

private:
	//基準位置となる対象
	std::weak_ptr<Actor> target_;
	//発生までのディレイ
	float delay_;
	//位置の加算値
	Vector2 addVec_;
	//MetamorEffectが何個作られたかをカウントする
	int effectCount_{ 0 };
};
