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
	// コンストラクタ
	CreditScene();
	// デストラクタ
	~CreditScene();

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
	//画面外か？
	bool ScreenOut() const;
	//プレイヤーリスタート
	void PlayerRestart();
	//リスタートセット	
	void RestartSet();
	//操作不能時
	void NotOperate();
	//スタート時
	void PlayerStart();

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<CreditPlayer>;
	using CreditPtr = std::shared_ptr<CreditText>;

	// ワールド
	WorldPtr		world_;
	PlayerPtr		player_;
	CreditPtr		credit_;

	BackgroundScreen bgScreen_;

	//ステージ作成マネージャー
	StageGenerateManager stageGeneratorManager;

	CharacterAnmManager anmManager_;

	PlayerInputChecker isUseKey_;

	// 次のシーン
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	bool isRetry_;

	//
	const float SceneTime = 54.0f;		//遷移するまでの時間
	float sceneTimer_;					//遷移するまでのカウント
	Vector2 pHeadPos_;					//頭座標
	Vector2 startPos_;					//スタート座標
	bool operate_;						//操作可能か
	Vector2 correction;					//補正値
	std::array<Vector2, 2> whitePos_;	//白カラスの目的地座標
	Vector2 dWhitePos_;					//白カラスの描画座標
	Vector2 wCorr;						//白カラスの補正値
	bool sceneChange_;					//遷移可能か
	bool mulInit_;						//多重振り子初期化
	bool waiting_;						//待機中
};