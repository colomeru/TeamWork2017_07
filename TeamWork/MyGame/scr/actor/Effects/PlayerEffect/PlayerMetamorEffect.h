#pragma once
#include"../Effect.h"

class PlayerMetamorEffect:public Effect{
public:
	PlayerMetamorEffect(IWorld* world, const Vector2& position,const std::shared_ptr<Actor>& target, float delay=0.0f, const Vector2& addVec=Vector2::Zero);

	void Update()override;
	void Draw()const override;
	void OnMessage(EventMessage message, void * param)override;

private:
	//��ʒu�ƂȂ�Ώ�
	std::weak_ptr<Actor> target_;
	//�����܂ł̃f�B���C
	float delay_;
	//�ʒu�̉��Z�l
	Vector2 addVec_;
	//MetamorEffect�������ꂽ�����J�E���g����
	int effectCount_{ 0 };
};
