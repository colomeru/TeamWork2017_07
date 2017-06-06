#pragma once
#include "IScene.h"
#include <map>
#include"../actor/CameraAct/TPSCamera.h"
#include"../stageGenerator/StageGeneratorManager.h"
#include"addScreen/GameOverScreen.h"
#include"addScreen/StartScreen.h"
#include"addScreen/BackgroundScreen.h"
#include"addScreen/GameClearScreen.h"
#include"addScreen/PauseScreen.h"
#include"addScreen\LaneChangeScreen.h"
#include"addScreen\StageEffectScreen.h"

class Player;
class EnemyGenerator;

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
	
	void ReceiveStage(Stage stage)override {
		currentStage_ = stage;
	}
private:
	void setNextMode(int mode) {
		gamePlayMode_ = mode;
		switch (gamePlayMode_)
		{
		case 2: {
			gameOverScreen_.Init();
			break;
		}
		case 3: {
			gameClearScreen_.Init();
			break;
		}
		case 4: {
			pauseScreen_.Init();
		}
		default:
			break;
		}
	}
private:
	void startUpdate();
	void baseUpdate();
	void pauseUpdate();
	void overUpdate();
	void clearUpdate();
	void nextUpdate();
	void nextSwitchUpdate();


private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;

	PlayerPtr ply1;
	std::shared_ptr<EnemyGenerator> enemGenerator_;
	StageGenerateManager stageGeneratorManager;
	StartScreen startScreen_;
	PauseScreen pauseScreen_;
	GameOverScreen gameOverScreen_;
	GameClearScreen gameClearScreen_;
	BackgroundScreen bgScreen_;
	LaneChangeScreen changeScreen_;
	StageEffectScreen stageEffectScreen_;
	//Vector3 posit;

	Stage currentStage_;

	//Vector3 camera_pos_;
	//Vector3 target_;
	int windTime_;

	//���̃X�e�[�W�̃��[���̍ő吔
	int maxLaneCount;
	float stageLen_;
	float meterLen_;
	Vector2 meterPos_;

	//0=Start,1=Gameplay,2=Gameover,3=Gameclear,4=Pause
	int	gamePlayMode_;

	std::map<Stage,Stage> nextStageList_;

	std::map<Stage, int> defWindTime_;

	std::map<int, std::function<void()>> updateFunctionMap_;

};