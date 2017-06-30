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
#include "../actor/player/CreditPlayer.h"
#include <array>
#include "addScreen\screenSupport\CreditText.h"
#include "../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class Credit2Scene : public IScene
{
	enum {
		RESTART = 0,
		BITE = 1,
		FALL = 2,
	};

public:
	// �R���X�g���N�^
	Credit2Scene();
	// �f�X�g���N�^
	~Credit2Scene();

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
	//�X�N���[��
	void Scroll();
	//�摜�ɐG��Ă��邩�H
	bool IsCollision();
	//����\���H
	bool GetOperate();


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

	// ���̃V�[��
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	bool isRetry_;

	//
	Vector2 pHeadPos_;
	Vector2 startPos_;
	std::array<Vector2, 6> spritePos_;
	Vector2 spriteSize_;
	bool operate_;
	int playerStatte_;
	Vector2 correction;
	const float SceneTime = 54.0f;
	float sceneTimer_;
	int sinCount_;
	float alpha_;
	std::array<Vector2, 2> whitePos_;
	Vector2 dWhitePos_;
	Vector2 wCorr;
	bool waiting_;
	

	bool test;
	bool test2;
	bool test3;
};