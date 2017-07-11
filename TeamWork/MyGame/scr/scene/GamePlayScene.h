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
	//風を発生させる
	void BeginWind();
	//風の生成タイマーを初期化する
	void InitWindTime();
	//風の生成タイマーを再設定する
	void ResetWindTime();
private:
	//ポーズからゲーム画面に戻る際、そのまま再開するか、最初に戻るかを調べる
	void pause_Check_Resume(PauseScreen::returnGameType backType);

	//通常クリア時のUpdate
	void clear_Normal_Update();
	//全クリア時のUpdate
	void clear_All_Update();

private:
	void ToOverMode();
	void ToClearMode();
	void ToPauseMode();

	Stage AddStageNum(Stage current);
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

	//ゲームプレイに重ねる画面(追加描画)を管理する各種クラス
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

	//そのステージのレーンの最大数
	int maxLaneCount;
	float stageLen_;

	int changeCount_;

	MethodTimer windTimer_;

	//0=Start,1=Gameplay,2=Gameover,3=Gameclear,4=Pause
	int	gamePlayMode_;

	std::map<Stage,BGM_ID> stageBGMList_;

	std::map<Stage, int> defWindTime_;

	std::map<int, std::function<void()>> updateFunctionMap_;

};