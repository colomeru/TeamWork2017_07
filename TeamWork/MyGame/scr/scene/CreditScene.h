#pragma once

#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "GamePlayDefine.h"
#include "addScreen/BackgroundScreen.h"
#include "addScreen/TutorialTextScreen.h"
#include <array>
#include "../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"
#include "../actor/player/Input/PlayerInputChecker.h"

class CreditPlayer;
class CreditText;

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
	//��ʊO���H
	bool ScreenOut() const;
	//�v���C���[���X�^�[�g
	void PlayerRestart();
	//���X�^�[�g�Z�b�g	
	void RestartSet();
	//����s�\��
	void NotOperate();
	//�X�^�[�g��
	void PlayerStart();

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<CreditPlayer>;
	using CreditPtr = std::shared_ptr<CreditText>;

	// ���[���h
	WorldPtr		world_;
	PlayerPtr		player_;
	CreditPtr		credit_;

	BackgroundScreen bgScreen_;

	//�X�e�[�W�쐬�}�l�[�W���[
	StageGenerateManager stageGeneratorManager;

	CharacterAnmManager anmManager_;

	PlayerInputChecker isUseKey_;

	// ���̃V�[��
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	bool isRetry_;

	//
	const float SceneTime = 54.0f;		//�J�ڂ���܂ł̎���
	float sceneTimer_;					//�J�ڂ���܂ł̃J�E���g
	Vector2 pHeadPos_;					//�����W
	Vector2 startPos_;					//�X�^�[�g���W
	bool operate_;						//����\��
	Vector2 correction;					//�␳�l
	std::array<Vector2, 2> whitePos_;	//���J���X�̖ړI�n���W
	Vector2 dWhitePos_;					//���J���X�̕`����W
	Vector2 wCorr;						//���J���X�̕␳�l
	bool sceneChange_;					//�J�ډ\��
	bool mulInit_;						//���d�U��q������
	bool waiting_;						//�ҋ@��
};