#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>
#include"../tween/TweenManager.h"

const int SceneManager::MaxStageCount = 6;

// コンストラクタ
SceneManager::SceneManager() :
	mStageCount(6), nectSceneName_(Scene::Demo)
{

}

// 更新前初期化
void SceneManager::Initialize()
{
	End();
	mScenes.clear();
	TweenManager::GetInstance().Initialize();
	timer = 0;
	FadeTrigger = false;
	FadePanel::GetInstance().Initialize();
	FadePanel::GetInstance().SetInTime(0.0f);
	FadePanel::GetInstance().FadeIn();
}

// 更新
void SceneManager::Update()
{

	if (nectSceneName_ != Scene::Demo) {
		timer += Time::DeltaTime;
	}

	//timer += Time::DeltaTime;
	//if (!FadePanel::GetInstance().IsAction())
		mCurrentScene->Update();
	if (Keyboard::GetInstance().AnyTriggerDown()
		|| GamePad::GetInstance().AnyTriggerDown()) {
		timer = 0;
	}
	TweenManager::GetInstance().Update(Time::DeltaTime);
	TweenManager::GetInstance().Remove();
	FadePanel::GetInstance().Update(Time::DeltaTime);
}

// 描画
void SceneManager::Draw() const
{
	//if (!FadePanel::GetInstance().IsAction())
	mCurrentScene->Draw();
	FadePanel::GetInstance().Draw();
}

// 終了
void SceneManager::End()
{
	mCurrentScene->End();
	TweenManager::GetInstance().Clear();
}
// シーン変更
void SceneManager::Change()
{
	/*FadePanel::GetInstance().AddCollBack([=]() {
				
	});*/
	if (FadeTrigger == false)
	{
		if (timer >= 60.0f && nectSceneName_ == Scene::Title) {
			FadeTrigger = true;
			FadePanel::GetInstance().AddCollBack([=]() {
				SetBackgroundColor(153, 204, 255);
				Change(Scene::Movie);
			});
			FadePanel::GetInstance().SetOutTime(1.0f);
			FadePanel::GetInstance().FadeOut();
			timer = 0;
			FadeTrigger = false;
			return;
			
		}
		else if (timer >= 180.0f && nectSceneName_ != Scene::Movie) {
			FadeTrigger = true;
			FadePanel::GetInstance().AddCollBack([=]() {
				SetBackgroundColor(153, 204, 255);
				Change(Scene::Title);
			});
			FadePanel::GetInstance().SetOutTime(1.0f);
			FadePanel::GetInstance().FadeOut();
			timer = 0;			
			FadeTrigger = false;
			return;
		}
	}
	if (mCurrentScene->IsEnd())
	{
		Change(mCurrentScene->Next());
		timer = 0;
	}
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
	//if (FadePanel::GetInstance().IsAction())return;
	Scene now = Scene::None;
	for (auto& scene : mScenes)
	{
		if (mCurrentScene == scene.second)
			now = scene.first;
	}
	if (name == now)
		return;

	nectSceneName_ = name;

	//SetFadeInOutSpeed();
	//FadePanel::GetInstance().AddCollBack([this] {End(); });
	//FadePanel::GetInstance().AddCollBack([this] {SceneChangeAfterFade(); });
	//FadePanel::GetInstance().FadeOut();
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

void SceneManager::SceneChangeAfterFade()
{
	Stage targetStage = mCurrentScene->SendStage();
	mCurrentScene = mScenes[nectSceneName_];
	mCurrentScene->ReceiveStage(targetStage);
	mCurrentScene->Initialize();

	//FadePanel::GetInstance().Initialize();	
	//FadePanel::GetInstance().AddCollBack([this] {mCurrentScene->Initialize(); });
	FadePanel::GetInstance().FadeIn();

}

void SceneManager::SetFadeInOutSpeed()
{
	FadePanel::GetInstance().Initialize();
	FadePanel::GetInstance().SetOutTime(0.2f);
	FadePanel::GetInstance().SetInTime(0.4f);

}
