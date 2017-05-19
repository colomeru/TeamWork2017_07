#include "MenuScene.h"
#include "../time/Time.h"
#include "../math/MathHelper.h"
#include "../graphic//Sprite.h"

MenuScene::MenuScene() :
nextScene_(Scene::GamePlay)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
	isEnd_ = false;

	for (int i = 0; i < 9; i++)
	{
		stagePos[i] = Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f);
	}

	//
	dis = disN = 0.0f;
	pos = Vector2(WINDOW_WIDTH / 2.0f, height - 50.0f);
	gPos = Vector2(WINDOW_WIDTH / 2.0f, height - 50.0f);
}

void MenuScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP))
	{
		stageNum++;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN))
	{
		stageNum--;
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	//
	dir = gPos - pos;
	dis = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	disN = 1.0f / dis;
	dir.x *= disN;
	dir.y *= disN;
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::V))
	{
		gPos.y -= 150.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::B))
	{
		pos = pos + dir;
	}
}

void MenuScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "MenuScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);

	int panelNum, cursorNum;
	if (stageNum < 5)
	{
		panelNum = stageNum;
		cursorNum = 0;
	}
	else
	{
		panelNum = 4;
		cursorNum = stageNum - 4;
	}

	//ステージパネルを描画
	for (int i = 0; i < 9;i++)
	{
		auto drawPos = stagePos[i] + Vector2(0, 150 * panelNum);
		auto min = drawPos - Vector2(300.0f, 50.0f);
		auto max = drawPos + Vector2(300.0f, 50.0f);
		DrawBox(min.x, min.y,max.x,max.y,GetColor(0,255 - 20 * i,20 * i),1);
	}
	//戻るパネルを描画
	DrawBox(0,WINDOW_HEIGHT - 100.0f,300,WINDOW_HEIGHT,GetColor(255,0,0),1);

	//カーソルを描画
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(stagePos[cursorNum].x - 350, stagePos[cursorNum].y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RSHIFT))
	{
		DrawFormatString(stagePos[cursorNum].x - 280, stagePos[cursorNum].y, GetColor(255, 0, 0), "ステージ%dを選択したよ！", stageNum + 1);
	}

	//
	DrawCircle(pos.x, pos.y, 16, GetColor(0, 255, 0), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);


	// 描画
	world_->Draw();
}

bool MenuScene::IsEnd() const
{
	return isEnd_;
}

Scene MenuScene::Next() const
{
	return nextScene_;
}

void MenuScene::End()
{
	// 初期化
	world_->Clear();
}

void MenuScene::handleMessage(EventMessage message, void * param)
{
}
