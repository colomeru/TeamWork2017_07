#pragma once
#include "IScene.h"
#include <map>
#include"../actor/CameraAct/TPSCamera.h"
#include"../stageGenerator/StageGeneratorManager.h"
#include"addScreen/GameOverScreen.h"

class Player;

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
	
private:
	void baseUpdate();
	void clearUpdate();

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;

	PlayerPtr ply1;

	StageGenerateManager stageGeneratorManager;
	GameOverScreen gameOverScreen_;
	//Vector3 posit;

	//Vector3 camera_pos_;
	//Vector3 target_;
	int windTime_;

	//そのステージのレーンの最大数
	int maxLaneCount;
	float stageLen_;
	float meterLen_;
	Vector2 meterPos_;

	bool isPlayerDead_;

	std::map<bool, std::function<void()>> updateFunctionMap_;

};