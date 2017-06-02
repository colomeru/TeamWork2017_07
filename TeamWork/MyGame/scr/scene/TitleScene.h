#pragma once

#include "IScene.h"
#include "../time/Time.h"

class TitleScene : public IScene
{
public:
	// �R���X�g���N�^
	TitleScene();
	// �f�X�g���N�^
	~TitleScene();
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
	int selectNum_;
	float selectX_;
	float selectY_;
private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;
	
	// ���̃V�[��
	Scene			nextScene_;
};