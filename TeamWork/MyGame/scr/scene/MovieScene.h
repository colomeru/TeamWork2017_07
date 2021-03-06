#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"
#include "../graphic/Movie.h"
#include "../game/WorkFolder.h"

class MovieScene : public IScene
{
public:
	// コンストラクタ
	MovieScene();
	// デストラクタ
	~MovieScene();
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
	Movie movieView_;
	float timer;
	Scene			nextScene_;
};