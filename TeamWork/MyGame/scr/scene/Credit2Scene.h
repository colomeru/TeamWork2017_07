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
	// コンストラクタ
	Credit2Scene();
	// デストラクタ
	~Credit2Scene();

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
	//スクロール
	void Scroll();
	//画像に触れているか？
	bool IsCollision();
	//操作可能か？
	bool GetOperate();


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

	// 次のシーン
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