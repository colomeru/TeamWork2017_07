#pragma once

#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "GamePlayDefine.h"
#include "addScreen\BackgroundScreen.h"
#include "addScreen/TutorialTextScreen.h"

class TutorialScene : public IScene
{
public:
	// �R���X�g���N�^
	TutorialScene();
	// �f�X�g���N�^
	~TutorialScene();

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
	using PlayerPtr = std::shared_ptr<Player>;
	// ���[���h
	WorldPtr		world_;
	PlayerPtr		player_;

	BackgroundScreen bgScreen_;

	//�X�e�[�W�쐬�}�l�[�W���[
	StageGenerateManager stageGeneratorManager;

	// ���̃V�[��
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	bool isRetry_;
};