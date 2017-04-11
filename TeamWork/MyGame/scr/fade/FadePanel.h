#pragma once
#include "../Def.h"
#include "../math/Vector2.h"
#include <stack>

class FadePanel
{
public:
	enum ColorType
	{
		White,
		Black
	};

public:
	enum class FADE_STATUS
	{
		STANDBY = 0,	// ���s�҂�
		FadeIn	= 1,	// �t�F�[�h�C��
		FadeOut = 2		// �t�F�[�h�A�E�g
	};


private:
	// �R���X�g���N�^
	FadePanel();
	// �f�X�g���N�^
	~FadePanel();

public:
	static FadePanel &GetInstance(){
		static FadePanel f;
		return f;
	}

	// ������
	void Initialize();
	// �X�V
	void Update(float deltaTime);
	// �`��
	void Draw() const;
	// �t�F�[�h�C��
	void FadeIn();
	// �t�F�[�h�A�E�g
	void FadeOut(ColorType type = ColorType::Black, float maxAlpha = 1.0f);
	// �ψْ����H
	bool IsAction() const;
	// ��ʂ����܂��Ă��邩�H
	bool IsFillScreen() const;
	// ��ʂ��������H
	bool IsClearScreen() const;

	// �t�F�[�h�C���^�C���̎擾
	float GetInTime() const;
	// �t�F�[�h�C���^�C���̐ݒ�
	void  SetInTime(float sec);
	// �t�F�[�h�A�E�g�^�C���̎擾
	float GetOutTime() const;
	// �t�F�[�h�A�E�g�^�C���̐ݒ�
	void  SetOutTime(float sec);

	// �f�B���C�^�C���擾
	float GetDelayTime() const;
	// �f�B���C�^�C���ݒ�
	void  SetDelayTime(float sec);

private:
	// �t�F�[�h�C���A�b�v�f�[�g
	void FadeInUpdate(float deltaTime);
	// �t�F�[�h�A�E�g�A�b�v�f�[�g
	void FadeOutUpdate(float deltaTime);

private:
	// ���\�[�X�T�C�Y
	const Vector2 RES_SIZE = Vector2(800, 600);
	// �X�N���[���T�C�Y
	const Vector2 SCALE	   = Vector2((float)WINDOW_WIDTH / RES_SIZE.x, (float)WINDOW_HEIGHT / RES_SIZE.y);

	// �X�e�[�g�X�^�b�N
	std::stack<FADE_STATUS>	stateStack_;

	// �^�C�v
	ColorType	color_;
	// �A���t�@�l
	float		alpha_;
	// �A���t�@�ő�l
	float		maxAlpha_;
	// �ψَ���
	float		actionTime_;
	// �t�F�[�h�C���^�C��
	float		inTime_;
	// �t�F�[�h�A�E�g�^�C��
	float		outTime_;
	// �f�B���C�^�C��
	float		delayTime_;
};