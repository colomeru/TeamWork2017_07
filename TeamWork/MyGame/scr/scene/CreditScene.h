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
	//背景色変更
	void ChangeBackGroundColor();

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

	const Vector2 Corr = Vector2(300.0f, 500.0f);									//補正値
	const Vector2 WhiteCorr = Vector2(-40, -20);									//白カラスの補正値
	const Vector2 startPos_ = Vector2(WINDOW_WIDTH / 2.0f - Corr.x, -110.0f);		//スタート座標
	const float SceneTime = 54.0f;													//遷移するまでの時間

	float sceneTimer_;					//遷移するまでのカウント
	Vector2 pHeadPos_;					//頭座標
	bool operate_;						//操作可能か
	std::array<Vector2, 2> whitePos_;	//白カラスの目的地座標
	Vector2 dWhitePos_;					//白カラスの描画座標
	bool mulInit_;						//多重振り子初期化用
	bool waiting_;						//待機中
	int bgColorNum_;					//背景色番号
	float from_;						//Tween用
	Vector3 bgColor_;					//背景色
};