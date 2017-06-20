#define NOMINMAX
#include "SceneManager.h"
#include "IScene.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include <algorithm>
#include"../tween/TweenManager.h"

// �R���X�g���N�^
SceneManager::SceneManager() :
	isEnd_(false),
	timer_(0.0f),
	freeTime_(60.0f)
{
}

// �X�V�O������
void SceneManager::Initialize()
{
	End();
	sceneMap_.clear();
}

// �X�V
void SceneManager::Update()
{
	currentScene_->Update();
	FadePanel::GetInstance().Update(Time::DeltaTime);

	TweenManager::GetInstance().Update(Time::DeltaTime);
	TweenManager::GetInstance().Remove();

	// ���u�����Z�b�g
	if (Keyboard::GetInstance().AnyTriggerDown() || Keyboard::GetInstance().AnyStateDown() ||
		GamePad::GetInstance().AnyTriggerDown() || GamePad::GetInstance().AnyStateDown())
		SceneReset();
}

// �`��
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

// �I��
void SceneManager::End()
{
	currentScene_->End();
	TweenManager::GetInstance().Clear();
}

// �V�[���ύX
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

// �V�[���̒ǉ�
void SceneManager::Add(Scene name, const IScenePtr & scene)
{
	sceneMap_[name] = scene;
}

void SceneManager::SetScene(Scene name)
{
	currentScene_ = sceneMap_[name];
	currentScene_->Initialize();
}

// �V�[���ύX
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

// ���������w�肷��
void SceneManager::Init(Scene name)
{
	sceneMap_.at(name)->Initialize();
}

// �I���������w�肷��
void SceneManager::Final(Scene name)
{
	sceneMap_.at(name)->End();
}

void SceneManager::SceneReset()
{
	// �R���ԕ��u���ꂽ�烊�Z�b�g
	TweenManager::GetInstance().DelayCancel(&timer_);
	TweenManager::GetInstance().Delay(freeTime_, [=]() {
		FadePanel::GetInstance().SetOutTime(0.5f);
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}, &timer_);
}
