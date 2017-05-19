#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"

class MenuScene : public IScene
{
public:
	// コンストラクタ
	MenuScene();
	// デストラクタ
	~MenuScene();

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


	struct PanelStruct
	{
		Vector2 position;
		bool isDraw;
		float alpha;
	};

private:
	int stageNum = 0;
	const float height = WINDOW_HEIGHT / 4.0f * 3.0f;
	std::array<Vector2, 9> stagePos;

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir;
	float dis;
	float disN;

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;
};