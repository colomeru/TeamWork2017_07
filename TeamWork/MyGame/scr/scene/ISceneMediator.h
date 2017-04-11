#pragma once

enum class Scene;

class ISceneMediator
{
public:
	// ���z�f�X�g���N�^
	virtual ~ISceneMediator() = default;

	// �V�[���`�F���W
	virtual void Change(Scene name) = 0;
	// ������
	virtual void Init(Scene name)   = 0;
	// �I������
	virtual void Final(Scene name)  = 0;

	virtual int GetStageCount() const = 0;
	virtual void SetStageCount(int n) = 0;
};