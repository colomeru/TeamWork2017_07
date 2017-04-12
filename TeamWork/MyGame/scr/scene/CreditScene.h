#pragma once
#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector3.h"

class CreditScene : public IScene
{
public:
	// �R���X�g���N�^
	CreditScene();
	// �f�X�g���N�^
	~CreditScene();

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
	// ���[���h
	WorldPtr		world_;

	Vector3 camera_pos_;
	Vector3 target_;

	// ���̃V�[��
	Scene			nextScene_;
};