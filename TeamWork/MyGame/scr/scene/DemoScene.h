#pragma once

#include "IScene.h"

class DemoScene : public IScene
{
public:
	// コンストラクタ
	DemoScene();
	// デストラクタ
	~DemoScene();

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
	// 終了フラグ
	bool isEnd_;

	// 非同期読み込みコンテント数
	float	maxLoadContentCount_;
	// 読み込み中コンテント数
	float	currentLoadCount_;
	// 前フレーム読み込み数
	float	prevLoadCount_;

	int timeCount_;
	
	int LoadWashMachineFrame_;
	std::vector<SPRITE_ID> wMachineFrame_;
};