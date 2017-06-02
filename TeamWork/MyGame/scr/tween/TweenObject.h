#pragma once
#include <functional>

enum UpdateType
{
	Common,		// �ʏ�X�V
	Loop,		// ���[�v�X�V
	PingPong	// ���ꃋ�[�v�X�V
};

class TweenObject
{
public:
	// �R���X�g���N�^
	TweenObject(float* value, float b, float c, float d = 1.0f, const std::function<void()>& callback = nullptr, float s = 1.0f);
	// �f�X�g���N�^
	~TweenObject();
	// �X�V
	void Update(const float deltaTime);
	// �I�����Ă��邩�H
	bool IsEnd() const;
	// �I��
	void End();
	// �ϓ��l�|�C���^�擾
	float* GetValuePointer() const;

	// �֐��o�^
	void SetFunction(std::function<float(float, float, float, float, float)> func);
	// ���[�v�^�C�v�o�^
	void SetLoopType(const UpdateType type);

private:
	// �ʏ�X�V
	void CommonUpdate(float deltaTime);
	// ���[�v�X�V
	void LoopUpdate(float deltaTime);
	// ���ꃋ�[�v�X�V
	void PingPongUpdate(float deltaTime);
	// �R�[���o�b�N���s
	void Callback();

private:
	// ease�֐�
	std::function<float(float, float, float, float, float)> func_;
	// �X�V�֐�
	std::function<void(float)> updateFunc_;

	// �ϓ��l
	float*	value_;
	// �J�n�l
	float	b_;
	// �ړ���
	float	c_;
	// �I������
	float	d_;
	// Back�p
	float	s_;
	// �R�[���o�b�N
	std::function<void()> callback_;
	// ����
	float	timer_;
	// �I���t���O
	bool	isEnd_;
	
	// ���[�v�^�C�v
	UpdateType	loopType_;
	// �X�V���ėp�␳�l
	float	sine_;
};
