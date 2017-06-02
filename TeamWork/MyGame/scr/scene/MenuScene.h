#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"
#include "../scene/addScreen/MenuScreen.h"
#include "../time/Time.h"

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

private:
	MenuScreen menu;
private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;
};