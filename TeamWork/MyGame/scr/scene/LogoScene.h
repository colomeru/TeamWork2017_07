#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"
#include "../scene/addScreen/MenuScreen.h"
#include "../time/Time.h"

class LogoScene : public IScene
{
public:
	// �R���X�g���N�^
	LogoScene();
	// �f�X�g���N�^
	~LogoScene();

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
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	float timeCount_;

	// ���̃V�[��
	Scene			nextScene_;
};