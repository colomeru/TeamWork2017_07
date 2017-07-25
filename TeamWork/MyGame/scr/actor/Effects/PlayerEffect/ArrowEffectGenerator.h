#pragma once
#include"ArrowEffectGenerator.h"

class IWorld;
class Actor;
class ArrowEffectGenerator {
public:

	ArrowEffectGenerator();

	void Initialize(IWorld* world, Actor* target, float delayTime);
	void Update();

	//�G�t�F�N�g�𐶐����邩�ǂ��������肵�܂�
	void SetIsCreateEffect(bool isCreate);
	//�G�t�F�N�g�̐����^�C�}�[�����Z�b�g���܂�
	void ResetTime();

	//�G�t�F�N�g�̐������J�n���܂�
	void StartEffect();
	//�G�t�F�N�g�̐������I�����܂�
	void EndEffect();
private:
	//��������
	float delayTime_{ 0.f };
	float timeCount_{ 0.f };
	bool isCreateEffect_{ false };
	IWorld* world_;
	Actor* target_;
};