#pragma once

#include "IScene.h"

class DemoScene : public IScene
{
public:
	// �R���X�g���N�^
	DemoScene();
	// �f�X�g���N�^
	~DemoScene();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() const override;
	// �I�����Ă��邩�H
	virtual bool IsEnd() const override;
	// ���̃V�[����Ԃ�
	virtual Scene Next() const override;
	// �I��������
	virtual void End() override;

private:
	// �I���t���O
	bool isEnd_;

	// �񓯊��ǂݍ��݃R���e���g��
	float	maxLoadContentCount_;
	// �ǂݍ��ݒ��R���e���g��
	float	currentLoadCount_;
	// �O�t���[���ǂݍ��ݐ�
	float	prevLoadCount_;
};