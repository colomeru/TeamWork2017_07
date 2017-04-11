#pragma once

enum class Scene;

class ISceneMediator
{
public:
	// 仮想デストラクタ
	virtual ~ISceneMediator() = default;

	// シーンチェンジ
	virtual void Change(Scene name) = 0;
	// 初期化
	virtual void Init(Scene name)   = 0;
	// 終了処理
	virtual void Final(Scene name)  = 0;

	virtual int GetStageCount() const = 0;
	virtual void SetStageCount(int n) = 0;
};