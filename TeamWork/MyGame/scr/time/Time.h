#pragma once
#include "FPS.h"

class Time
{
public:
	Time() = default;

	// ������
	void Initialize();
	// �X�V����
	void Update();
	
public:
	// �O�t���[���ƍ��t���[���̎��ԍ�
	static float DeltaTime;

private:
	// �O�t���[���̎���
	int prevTime;
};