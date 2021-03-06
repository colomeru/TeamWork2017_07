#pragma once
#include"ArrowEffectGenerator.h"
#include"../../../world/IWorld.h"

class ArrowEffectGenerator {
public:

	ArrowEffectGenerator();

	void Initialize(IWorld* world, Actor* target, float delayTime);
	void Update();

	//エフェクトを生成するかどうかを決定します
	void SetIsCreateEffect(bool isCreate);
	//エフェクトの生成タイマーをリセットします
	void ResetTime();

	//エフェクトの生成を開始します
	void StartEffect();
	//エフェクトの生成を終了します
	void EndEffect();
private:
	//生成周期
	float delayTime_;
	float timeCount_;
	bool isCreateEffect_;
	IWorld* world_;
	Actor* target_;
};