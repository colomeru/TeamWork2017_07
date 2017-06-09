#pragma once

#include "ISceneMediator.h"
#include "IScenePtr.h"
#include "Scene.h"
#include <unordered_map>

class SceneManager : public ISceneMediator
{
public:
	// コンストラクタ
	SceneManager();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw() const;
	// 終了時処理
	void End();
	// シーンチェンジ
	void Change();

	// シーンの追加
	void Add(Scene name, const IScenePtr& scene);
	void SetScene(Scene name);
	template<typename T, typename... Args>
	void Add(Scene name, Args&&... arg)
	{
		Add(name, std::make_shared<T>(arg...));
	}
	// シーンの変更
	virtual void Change(Scene name) override;
	// 初期化を指定する
	virtual void Init(Scene name) override;
	// 終了処理を指定する
	virtual void Final(Scene name) override;

	virtual int GetStageCount() const override
	{
		return mStageCount;
	}
	virtual void SetStageCount(int n) override;

	__declspec(property(get = GetStageCount, put = SetStageCount)) int StageCount;

private:
	void SceneChangeAfterFade();
	void SetFadeInOutSpeed();
private:
	// コピー禁止
	SceneManager(const SceneManager& other) = default;
	SceneManager& operator = (const SceneManager& other) = default;

public:
	static const int MaxStageCount;
	float timer;
	bool FadeTrigger;

private:
	// シーン
	std::unordered_map<Scene, IScenePtr> mScenes;
	// 現在のシーン
	IScenePtr	mCurrentScene;
	int			mStageCount;
	Scene nectSceneName_;
};