#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>

const int SceneManager::MaxStageCount = 6;

// コンストラクタ
SceneManager::SceneManager() :
mStageCount(6)
{
}

// 更新前初期化
void SceneManager::Initialize()
{
	End();
	mScenes.clear();
}

// 更新
void SceneManager::Update()
{
	mCurrentScene->Update();
	FadePanel::GetInstance().Update(Time::DeltaTime);
}

// 描画
void SceneManager::Draw() const
{
	mCurrentScene->Draw();
	FadePanel::GetInstance().Draw();
}

// 終了
void SceneManager::End()
{
	mCurrentScene->End();
}

// シーン変更
void SceneManager::Change()
{
	if (mCurrentScene->IsEnd())
		Change(mCurrentScene->Next());
}

// シーンの追加
void SceneManager::Add(Scene name, const IScenePtr & scene)
{
	mScenes[name] = scene;
}

void SceneManager::SetScene(Scene name)
{
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
}

// シーン変更
void SceneManager::Change(Scene name)
{
	Scene now = Scene::None;
	for (auto& scene : mScenes)
	{
		if (mCurrentScene == scene.second)
			now = scene.first;
	}
	if (name == now)
		return;

	End();
	mCurrentScene = mScenes[name];
         	mCurrentScene->Initialize();
}

// 初期化を指定する
void SceneManager::Init(Scene name)
{
	mScenes.at(name)->Initialize();
}

// 終了処理を指定する
void SceneManager::Final(Scene name)
{
	mScenes.at(name)->End();
}

void SceneManager::SetStageCount(int n)
{
	mStageCount = MathHelper::Clamp(n, 0, SceneManager::MaxStageCount);
}
