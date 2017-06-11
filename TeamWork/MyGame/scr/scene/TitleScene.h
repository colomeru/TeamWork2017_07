#pragma once

#include "IScene.h"
#include"addScreen\TitleBGScreen.h"

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
	void SetNextPanel();
private:
	int selectNum_;
	Vector2 selectPos_;
	float timer;
	float dummy_;
private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;
	
	TitleBGScreen bgScreen_;
	// ���̃V�[��
	Scene			nextScene_;

	std::vector<float> alpha_;
	int sinCount_;

	bool isPushKey_;
	bool isStartSetPanel_;

};