#pragma once

#include "IScene.h"

class DemoScene : public IScene
{
public:
	// �R���X�g���N�^
	DemoScene() = default;
	// �f�X�g���N�^
	~DemoScene() = default;

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
	float fpsTimer = 0;
	float fps = 0;
};