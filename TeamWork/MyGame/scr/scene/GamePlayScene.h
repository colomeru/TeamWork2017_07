#pragma once
#include "IScene.h"
#include <map>
#include"../actor/CameraAct/TPSCamera.h"
#include"../stageGenerator/StageGeneratorManager.h"
#include"addScreen/GameOverScreen.h"
#include"addScreen/StartScreen.h"
#include"addScreen/BackgroundScreen.h"

class Player;

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
	void baseUpdate();
	void clearUpdate();
	void startUpdate();

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;

	PlayerPtr ply1;

	StageGenerateManager stageGeneratorManager;
	StartScreen startScreen_;
	GameOverScreen gameOverScreen_;
	BackgroundScreen bgScreen_;
	//Vector3 posit;

	//Vector3 camera_pos_;
	//Vector3 target_;
	int windTime_;

	//���̃X�e�[�W�̃��[���̍ő吔
	int maxLaneCount;
	float stageLen_;
	float meterLen_;
	Vector2 meterPos_;

	//0=Start,1=Gameplay,2=Gameover
	int	gamePlayMode_;

	std::map<int, std::function<void()>> updateFunctionMap_;

};