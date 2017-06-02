#pragma once
#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "../actor/Field/Clothes/GoalClothes/GoalClothes.h"
#include "GamePlayDefine.h"
#include "addScreen\BackgroundScreen.h"

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

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	using GoalPtr = std::shared_ptr<GoalClothes>;
	// ワールド
	WorldPtr		world_;
	PlayerPtr		player_;
	GoalPtr			goal_;

	Vector3 camera_pos_;
	Vector3 target_;

	BackgroundScreen bgScreen_;

	//テスト用フラグ
	bool isTest_;

	Vector2 size;

	//ステージ作成マネージャー
	StageGenerateManager stageGeneratorManager;

	// 次のシーン
	Scene			nextScene_;
};