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

	// リソース関連初期化
	Model::GetInstance().Initialize();
	Sprite::GetInstance().Initialize();
	Sound::GetInstance().Initialize();
	FontManager::GetInstance().Initialize();

	mContent.NotAsyncLoadSprite(Sprite::GetInstance(), Model::GetInstance());

	// 非同期読み込み開始
	mContent.EnableASync();

	// ファイルの読み込み
	mContent.LoadSprite(Sprite::GetInstance(), Model::GetInstance());
	mContent.LoadModel(Model::GetInstance(), false);
	mContent.LoadSound(Sound::GetInstance());
	mContent.LoadFont(FontManager::GetInstance());

	// 非同期読み込み終了
	mContent.DisableASync();

	// 時間初期化
	mTime.Initialize();

	// シーンの追加
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

	// シーンの読み込み
	mSceneManager.SetScene(Scene::Demo);
}

void Game1::Update()
{
	// 時間を更新
	mTime.Update();

	// 入力を更新
	Keyboard::GetInstance().Update();
	GamePad::GetInstance().Update();

	// Escキー入力で強制終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::ESC))
	{
		GameFrame::GameEnd();
		return;
	}

	// シーンが変更されているかを確認
	mSceneManager.Change();
	// シーンを更新
	mSceneManager.Update();
	
	Sound::GetInstance().Update();
}

void Game1::Draw()
{
	// シーンを描画
	mSceneManager.Draw();
}

void Game1::Finalize()
{
	mSceneManager.End();
	DeleteShader(-1);
	DeleteGraph(-1);
}
