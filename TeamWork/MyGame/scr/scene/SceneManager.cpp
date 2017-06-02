#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>
#include"../tween/TweenManager.h"

const int SceneManager::MaxStageCount = 6;

// �R���X�g���N�^
SceneManager::SceneManager() :
mStageCount(6), nectSceneName_(Scene::Title)
{

}

// �X�V�O������
void SceneManager::Initialize()
{
	End();
	mScenes.clear();
	TweenManager::GetInstance().Initialize();
}

// �X�V
void SceneManager::Update()
{
	if (!FadePanel::GetInstance().IsAction()) 
		mCurrentScene->Update();
	
	TweenManager::GetInstance().Update(Time::DeltaTime);
	TweenManager::GetInstance().Remove();
	FadePanel::GetInstance().Update(Time::DeltaTime);
}

// �`��
void SceneManager::Draw() const
{
	//if (!FadePanel::GetInstance().IsAction())
		mCurrentScene->Draw();
	FadePanel::GetInstance().Draw();
}

// �I��
void SceneManager::End()
{
	mCurrentScene->End();
	TweenManager::GetInstance().Clear();
}

// �V�[���ύX
void SceneManager::Change()
{
	if (mCurrentScene->IsEnd())
		Change(mCurrentScene->Next());
}

// �V�[���̒ǉ�
void SceneManager::Add(Scene name, const IScenePtr & scene)
{
	mScenes[name] = scene;
}

void SceneManager::SetScene(Scene name)
{
	mCurrentScene = mScenes[name];
	mCurrentScene->Initialize();
}

// �V�[���ύX
void SceneManager::Change(Scene name)
{
	if (FadePanel::GetInstance().IsAction())return;
	Scene now = Scene::None;
	for (auto& scene : mScenes)
	{
		if (mCurrentScene == scene.second)
			now = scene.first;
	}
	if (name == now)
		return;

	nectSceneName_ = name;

	SetFadeInOutSpeed();
	FadePanel::GetInstance().AddCollBack([this] {End(); });
	FadePanel::GetInstance().AddCollBack([this] {SceneChangeAfterFade(); });
	FadePanel::GetInstance().FadeOut();
	//End();
	//mCurrentScene = mScenes[name];
 //        	mCurrentScene->Initialize();

}

// ���������w�肷��
void SceneManager::Init(Scene name)
{
	mScenes.at(name)->Initialize();
}

// �I���������w�肷��
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
	mCurrentScene = mScenes[nectSceneName_];
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
