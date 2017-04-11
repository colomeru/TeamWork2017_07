#pragma once

#include "IScene.h"

class DemoScene : public IScene
{
public:
	// コンストラクタ
	DemoScene() = default;
	// デストラクタ
	~DemoScene() = default;

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

private:
	float fpsTimer = 0;
	float fps = 0;
};