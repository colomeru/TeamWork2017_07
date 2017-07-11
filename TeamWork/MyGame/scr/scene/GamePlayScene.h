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
#include"../method/MethodTimer.h"

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
	//���𔭐�������
	void BeginWind();
	//���̐����^�C�}�[������������
	void InitWindTime();
	//���̐����^�C�}�[���Đݒ肷��
	void ResetWindTime();
private:
	//�|�[�Y����Q�[����ʂɖ߂�ہA���̂܂܍ĊJ���邩�A�ŏ��ɖ߂邩�𒲂ׂ�
	void pause_Check_Resume(PauseScreen::returnGameType backType);

	//�ʏ�N���A����Update
	void clear_Normal_Update();
	//�S�N���A����Update
	void clear_All_Update();

private:
	void ToStartMode();
	void ToBaseMode();
	void ToOverMode();
	void ToClearMode();
	void ToPauseMode();
	void ToNextMode();
	void ToSwitchMode();

	Stage AddStageNum(Stage current);

	void OverDraw()const;
	void ClearDraw()const;
	void PauseDraw()const;
	
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

	Stage currentStage_;

	int windTime_;

	//���̃X�e�[�W�̃��[���̍ő吔
	int maxLaneCount;
	float stageLen_;

	//���̐����^�C�}�[
	MethodTimer windTimer_;
	
	//�X�e�[�W�N���A�^�C�}�[
	MethodTimer clearTimer_;
	//0=Start,1=Gameplay,2=Gameover,3=Gameclear,4=Pause
	int	gamePlayMode_;

	std::map<Stage,BGM_ID> stageBGMList_;

	//�X�V���[�h��ύX���邽�߂̊֐����X�g
	std::map<int, std::function<void()>> changeModeFunctionMap_;

	//�X�V���[�h�̊֐����X�g
	std::map<int, std::function<void()>> updateFunctionMap_;

private:
	//�����I�ɃX�e�[�W��ύX�����l
	const int changeCount_{ 600 };

};