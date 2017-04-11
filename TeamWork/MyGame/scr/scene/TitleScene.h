#pragma once

#include "IScene.h"

class TitleScene : public IScene
{
public:
	// �R���X�g���N�^
	TitleScene() = default;
	// �f�X�g���N�^
	~TitleScene() = default;

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

};