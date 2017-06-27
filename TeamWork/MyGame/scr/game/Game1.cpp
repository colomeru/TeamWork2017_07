#include "Game1.h"
#include "../input/Keyboard.h"
#include "../input/GamePad.h"
#include "../scene/DemoScene.h"
#include "../scene/TitleScene.h"
#include "../scene/MenuScene.h"
#include "../scene/TutorialScene.h"
#include "../scene/GamePlayScene.h"
#include "../scene/CreditScene.h"
#include "../scene/EndingScene.h"
#include "../scene/MovieScene.h"
#include "../graphic/Anime.h"
#include "../graphic/Model.h"
#include "../graphic/Sprite.h"
#include "../sound/sound.h"
#include "../graphic/FontManager.h"
#include "../scene/Credit2Scene.h"
#include "../scene/LogoScene.h"


void Game1::Initialize()
{
	mIsLoaded = false;

	// ���\�[�X�֘A������
	Model::GetInstance().Initialize();
	Sprite::GetInstance().Initialize();
	Sound::GetInstance().Initialize();
	FontManager::GetInstance().Initialize();

	mContent.NotAsyncLoadSprite(Sprite::GetInstance(), Model::GetInstance());

	// �񓯊��ǂݍ��݊J�n
	mContent.EnableASync();

	// �t�@�C���̓ǂݍ���
	mContent.LoadSprite(Sprite::GetInstance(), Model::GetInstance());
	mContent.LoadModel(Model::GetInstance(), false);
	mContent.LoadSound(Sound::GetInstance());
	mContent.LoadFont(FontManager::GetInstance());

	// �񓯊��ǂݍ��ݏI��
	mContent.DisableASync();

	// ���ԏ�����
	mTime.Initialize();

	// �V�[���̒ǉ�
	mSceneManager.Add(Scene::Demo, std::make_shared<DemoScene>());
	mSceneManager.Add(Scene::Logo, std::make_shared<LogoScene>());
	mSceneManager.Add(Scene::Title, std::make_shared<TitleScene>());
	mSceneManager.Add(Scene::Menu, std::make_shared<MenuScene>());
	mSceneManager.Add(Scene::Tutorial, std::make_shared<TutorialScene>());
	mSceneManager.Add(Scene::GamePlay, std::make_shared<GamePlayScene>());
	mSceneManager.Add(Scene::Credit, std::make_shared<CreditScene>());
	mSceneManager.Add(Scene::Ending, std::make_shared<EndingScene>());
	mSceneManager.Add(Scene::Movie, std::make_shared<MovieScene>());
	mSceneManager.Add(Scene::Credit2, std::make_shared<Credit2Scene>());

	// �V�[���̓ǂݍ���
	mSceneManager.SetScene(Scene::Demo);
}

void Game1::Update()
{
	// ���Ԃ��X�V
	mTime.Update();

	// ���͂��X�V
	Keyboard::GetInstance().Update();
	GamePad::GetInstance().Update();

	// Esc�L�[���͂ŋ����I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::ESC))
	{
		GameFrame::GameEnd();
		return;
	}

	// �V�[�����ύX����Ă��邩���m�F
	mSceneManager.Change();
	// �V�[�����X�V
	mSceneManager.Update();
	
	Sound::GetInstance().Update();
}

void Game1::Draw()
{
	// �V�[����`��
	mSceneManager.Draw();
}

void Game1::Finalize()
{
	mSceneManager.End();
	DeleteShader(-1);
	DeleteGraph(-1);
}
