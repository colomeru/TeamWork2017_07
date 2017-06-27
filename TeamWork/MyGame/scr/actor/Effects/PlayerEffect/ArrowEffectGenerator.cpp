#include "ArrowEffectGenerator.h"
#include"../../../time/Time.h"
#include"CursorEffect.h"

ArrowEffectGenerator::ArrowEffectGenerator()
{

}


void ArrowEffectGenerator::Initialize(IWorld * world, Actor * target, float delayTime)
{
	world_=(world);
	target_=(target);
	delayTime_=(delayTime);
	isCreateEffect_ = (false);
}

void ArrowEffectGenerator::Update()
{
	if (!isCreateEffect_)return;

	timeCount_ += Time::DeltaTime;

	if (timeCount_ >= delayTime_) {
		ResetTime();
		
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<CursorEffect>(world_, target_->GetPosition(), target_, CursorEffect::Arrow::Left));
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<CursorEffect>(world_, target_->GetPosition(), target_, CursorEffect::Arrow::Right));
	}
}

void ArrowEffectGenerator::ResetTime()
{
	timeCount_ = 0.f;
}

void ArrowEffectGenerator::StartEffect()
{
	ResetTime();
	SetIsCreateEffect(true);
}

void ArrowEffectGenerator::EndEffect()
{
	SetIsCreateEffect(false);
}

void ArrowEffectGenerator::SetIsCreateEffect(bool isCreate)
{
	isCreateEffect_ = isCreate;
}
