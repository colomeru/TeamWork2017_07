#pragma once
#include "Scene.h"
#include "../world/World.h"
#include "../input/Keyboard.h"
#include "../input/GamePad.h"
#include"../stageGenerator/Stage.h"

// シーンインターフェース
class IScene
{
public:
	// 仮想デストラクタ
	virtual ~IScene() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新処理
	virtual void Update() = 0;
	// 描画処理
	virtual void Draw() const = 0;
	// 終了しているか？
	virtual bool IsEnd() const = 0;
	// 移行先のシーンを返す
	virtual Scene Next() const = 0;
	// 終了時処理
	virtual void End() = 0;
	virtual void ReceiveStage(Stage stage) {}
	virtual Stage SendStage()const {
		return Stage::Stage1;
	}
	void SetIsEnd(bool isEnd) {
		isEnd_ = isEnd;
	}

protected:
	bool	isEnd_;
};