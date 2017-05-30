#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"
#include "../graphic/Movie.h"
#include "../game/WorkFolder.h"

class MovieScene : public IScene
{
public:
	// �R���X�g���N�^
	MovieScene();
	// �f�X�g���N�^
	~MovieScene();
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
	Movie movieView_;

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;
};