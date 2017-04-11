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

	// リソース関連初期化
	Model::GetInstance().Initialize();
	Sprite::GetInstance().Initialize();

	// ファイルの読み込み
	mContent.LoadSprite(Sprite::GetInstance(), Model::GetInstance());
	mContent.LoadModel(Model::GetInstance(), false);

	// 時間初期化
	mTime.Initialize();

	// シーンの追加
	mSceneManager.Add(Scene::Demo, std::make_shared<DemoScene>());
	mSceneManager.Add(Scene::Title, std::make_shared<TitleScene>());
	mSceneManager.Add(Scene::Menu, std::make_shared<MenuScene>());
	mSceneManager.Add(Scene::GamePlay, std::make_shared<GamePlayScene>());
	mSceneManager.Add(Scene::Credit, std::make_shared<CreditScene>());
	mSceneManager.Add(Scene::Ending, std::make_shared<EndingScene>());

	// シーンの読み込み
	mSceneManager.SetScene(Scene::Demo);
}

void Game1::Update()
{
	// 時間を更新
	mTime.Update();

	// 入力を更新
	Keyboard::GetInstance().Update();

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
