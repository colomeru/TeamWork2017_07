#pragma once

class FPS
{
public:
	// �R���X�g���N�^
	FPS();

	// �X�V����
	void Update();
	// �ҋ@
	void Wait();

public:
	// FPS
	static float GetFPS;

private:
	// ����J�n
	int		startTime_;
	// �J�E���^
	int		count_;
	// fps
	float	fps_;

};