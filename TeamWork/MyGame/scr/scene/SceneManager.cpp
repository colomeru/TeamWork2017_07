#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>
#include"../tween/TweenManager.h"

// コンストラクタ
SceneManager::SceneManager() :
	isEnd_(false),
	timer_(0.0f),
	freeTime_(60.0f)
{
}

// 更新前初期化
void SceneManager::Initialize()
{
	End();
	sceneMap_.clear();
}

// 更新
void SceneManager::Update()
{
	currentScene_->Update();
	FadePanel::GetInstance().Update(Time::DeltaTime);

	TweenManager::GetInstance().Update(Time::DeltaTime);
	TweenManager::GetInstance().Remove();

	// 放置時リセット
	if (Keyboard::GetInstance().AnyTriggerDown() || Keyboard::GetInstance().AnyStateDown() ||
		GamePad::GetInstance().AnyTriggerDown() || GamePad::GetInstance().AnyStateDown())
		SceneReset();
}

// 描画
void SceneManager::Draw() const
{
	currentScene_->Draw();
	FadePanel::GetInstance().Draw();

	if (BuildMode == 1)
	{
		DrawFormatString(0, WINDOW_HEIGHT - 20, GetColor(255, 255, 255), "FPS:[%0.1f]", FPS::GetFPS);
		DrawFormatString(0, WINDOW_HEIGHT - 40, GetColor(255, 255, 255), "TIME_TO_LEAVE[%f]", timer_);
		DrawFormatString(0, WINDOW_HEIGHT - 60, GetColor(255, 255, 255), "TIME_TO_LEAVE[%d]", FadePanel::GetInstance().Count());
	}
}

// 終了
void SceneManager::End()
{
	currentScene_->End();
	TweenManager::GetInstance().Clear();
}

// シーン変更
void SceneManager::Change()
{
	if (currentScene_->IsEnd())
		Change(currentScene_->Next());

	if (isEnd_)
	{
		Scene now = Scene::None;
		for (auto& scene : sceneMap_)
		{
			if (currentScene_ == scene.second)
				now = scene.first;
		}
		if (now == Scene::Demo)
			return;

		if (now == Scene::Title)
			Change(Scene::Movie);
		else
			Change(Scene::Title);
		isEnd_ = false;
	}
}

// シーンの追加
void SceneManager::Add(Scene name, const IScenePtr & scene)
{
	sceneMap_[name] = scene;
}

void SceneManager::SetScene(Scene name)
{
	currentScene_ = sceneMap_[name];
	currentScene_->Initialize();
}

// シーン変更
void SceneManager::Change(Scene name)
{
	Scene now = Scene::None;
	for (auto& scene : sceneMap_)
	{
		if (currentScene_ == scene.second)
			now = scene.first;
	}
	if (name == now)
		return;

	End();
	currentScene_ = sceneMap_[name];
	currentScene_->Initialize();

	if (name == Scene::Title)
		freeTime_ = 60.0f;
	else if (name == Scene::Movie)
		return;
	else
		freeTime_ = 180.0f;

	SceneReset();
}

// 初期化を指定する
void SceneManager::Init(Scene name)
{
	sceneMap_.at(name)->Initialize();
}

// 終了処理を指定する
void SceneManager::Final(Scene name)
{
	sceneMap_.at(name)->End();
}

void SceneManager::SceneReset()
{
	// ３分間放置されたらリセット
	TweenManager::GetInstance().DelayCancel(&timer_);
	TweenManager::GetInstance().Delay(freeTime_, [=]() {
		FadePanel::GetInstance().SetOutTime(0.5f);
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}, &timer_);
}
