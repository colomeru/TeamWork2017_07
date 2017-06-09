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
	// コンストラクタ
	GamePlayScene();
	// デストラクタ
	~GamePlayScene();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() const override;
	// 終了しているか？
	virtual bool IsEnd() const override;
	// 次のシーンを返す
	virtual Scene Next() const override;
	// 終了時処理
	virtual void End() override;
	// メッセージ処理
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
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
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

	//そのステージのレーンの最大数
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