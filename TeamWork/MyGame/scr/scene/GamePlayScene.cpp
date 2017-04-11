#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"

#include "../actor/SampleActor.h"

GamePlayScene::GamePlayScene() :
nextScene_(Scene::Credit)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::Initialize()
{
	isEnd_ = false;

	// フェードパネル初期化
	FadePanel::GetInstance().Initialize();

	// アクター生成
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<SampleActor>(world_.get()));
}

void GamePlayScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void GamePlayScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "GamePlayScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// 描画
	world_->Draw();
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
	// 初期化
	world_->Clear();
}

void GamePlayScene::handleMessage(EventMessage message, void * param)
{
}
