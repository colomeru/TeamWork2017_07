#pragma once
#include "IScene.h"
#include <map>
#include"../actor/CameraAct/TPSCamera.h"

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
	using WorldPtr = std::shared_ptr<World>;
	using CameraPtr	= std::shared_ptr<TPSCamera>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;

	Vector3 posit;

	CameraPtr camera_;
	Vector3 camera_pos_;
	Vector3 target_;
};