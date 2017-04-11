#include "Game1.h"
#include "../input/Keyboard.h"
#include "../scene/DemoScene.h"
#include "../scene/TitleScene.h"
#include "../scene/MenuScene.h"
#include "../scene/GamePlayScene.h"
#include "../scene/CreditScene.h"
#include "../scene/EndingScene.h"

#include "../graphic/Anime.h"
#include "../graphic/Model.h"
#include "../graphic/Sprite.h"

void Game1::Initialize()
{
	mIsLoaded = false;

	// ���\�[�X�֘A������
	Model::GetInstance().Initialize();
	Sprite::GetInstance().Initialize();

	// �t�@�C���̓ǂݍ���
	mContent.LoadSprite(Sprite::GetInstance(), Model::GetInstance());
	mContent.LoadModel(Model::GetInstance(), false);

	// ���ԏ�����
	mTime.Initialize();

	// �V�[���̒ǉ�
	mSceneManager.Add(Scene::Demo, std::make_shared<DemoScene>());
	mSceneManager.Add(Scene::Title, std::make_shared<TitleScene>());
	mSceneManager.Add(Scene::Menu, std::make_shared<MenuScene>());
	mSceneManager.Add(Scene::GamePlay, std::make_shared<GamePlayScene>());
	mSceneManager.Add(Scene::Credit, std::make_shared<CreditScene>());
	mSceneManager.Add(Scene::Ending, std::make_shared<EndingScene>());

	// �V�[���̓ǂݍ���
	mSceneManager.SetScene(Scene::Demo);
}

void Game1::Update()
{
	// ���Ԃ��X�V
	mTime.Update();

	// ���͂��X�V
	Keyboard::GetInstance().Update();

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
