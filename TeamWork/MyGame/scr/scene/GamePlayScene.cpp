#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"


GamePlayScene::GamePlayScene() :
nextScene_(Scene::None)
{
}

GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::Initialize()
{
	isEnd_ = false;

	// 初期設定
	nextScene_		= Scene::Title;

	// フェードパネル初期化
	FadePanel::GetInstance().Initialize();

	//fieldWorld_->Add(ACTOR_ID::MESSAGE_WINDOW_ACTOR, std::make_shared<MessageWindow>(fieldWorld_.get()));
}

void GamePlayScene::Update()
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
	{
		isEnd_ = true;
	}
}

void GamePlayScene::Draw() const
{
}

bool GamePlayScene::IsEnd() const
{
	return isEnd_;
}

Scene GamePlayScene::Next() const
{
	return nextScene_;
}

void GamePlayScene::End()
{
}

void GamePlayScene::handleMessage(EventMessage message, void * param)
{
}
