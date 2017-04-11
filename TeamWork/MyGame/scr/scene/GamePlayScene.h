#pragma once
#include "IScene.h"
#include <map>

class GamePlayScene : public IScene
{	
public:
	// �R���X�g���N�^
	GamePlayScene();
	// �f�X�g���N�^
	~GamePlayScene();

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


private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr	= std::shared_ptr<World>;


	// ���̃V�[��
	Scene			nextScene_;
};