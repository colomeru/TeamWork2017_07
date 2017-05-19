#pragma once
#include "Scene.h"
#include "../world/World.h"
#include "../input/Keyboard.h"
#include "../input/GamePad.h"

// �V�[���C���^�[�t�F�[�X
class IScene
{
public:
	// ���z�f�X�g���N�^
	virtual ~IScene() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V����
	virtual void Update() = 0;
	// �`�揈��
	virtual void Draw() const = 0;
	// �I�����Ă��邩�H
	virtual bool IsEnd() const = 0;
	// �ڍs��̃V�[����Ԃ�
	virtual Scene Next() const = 0;
	// �I��������
	virtual void End() = 0;

protected:
	bool	isEnd_;
};