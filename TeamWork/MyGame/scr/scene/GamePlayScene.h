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
#include"addScreen/AllClearScreen.h"
#include"addScreen/UIScreen.h"

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
	void setNextMode(int mode);
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

	//�Q�[���v���C�ɏd�˂���(�ǉ��`��)���Ǘ�����e��N���X
	StartScreen startScreen_;
	PauseScreen pauseScreen_;
	GameOverScreen gameOverScreen_;
	GameClearScreen gameClearScreen_;
	BackgroundScreen bgScreen_;
	LaneChangeScreen changeScreen_;
	StageEffectScreen stageEffectScreen_;
	AllClearScreen allClearScreen_;
	UIScreen uiScreen_;
	//Vector3 posit;

	Stage currentStage_;

	//Vector3 camera_pos_;
	//Vector3 target_;
	int windTime_;

	//���̃X�e�[�W�̃��[���̍ő吔
	int maxLaneCount;
	float stageLen_;

	int changeCount_;

	//0=Start,1=Gameplay,2=Gameover,3=Gameclear,4=Pause
	int	gamePlayMode_;

	std::map<Stage, Stage> nextStageList_;

	std::map<Stage,BGM_ID> stageBGMList_;

	std::map<Stage, int> defWindTime_;
	
	std::map<Stage, int> stagenum_;

	std::map<int, std::function<void()>> updateFunctionMap_;


};