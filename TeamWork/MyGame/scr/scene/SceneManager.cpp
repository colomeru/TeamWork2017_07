#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>

const int SceneManager::MaxStageCount = 6;

// �R���X�g���N�^
SceneManager::SceneManager() :
mStageCount(6)
{
}

// �X�V�O������
void SceneManager::Initialize()
{
	End();
	mScenes.clear();
}

// �X�V
void SceneManager::Update()
{
	mCurrentScene->Update();
	FadePanel::GetInstance().Update(Time::DeltaTime);
}

// �`��
void SceneManager::Draw() const
{
	mCurrentScene->Draw();
	FadePanel::GetInstance().Draw();
}

// �I��
void SceneManager::End()
{
	mCurrentScene->End();
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
