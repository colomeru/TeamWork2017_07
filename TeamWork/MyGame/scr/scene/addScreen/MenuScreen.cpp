#include "MenuScreen.h"
#include "../../time/Time.h"
#include "../../math/MathHelper.h"
#include "../../graphic/Sprite.h"
#include "../../input/Keyboard.h"

//コンストラクタ
MenuScreen::MenuScreen()
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 0) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),false,0.0f };
	}
	backPos = Vector2(0.0f, WINDOW_HEIGHT - 100.0f);
	cursorPos = Vector2(panel[0].position.x - 350.0f, panel[0].position.y);

	//
	dis = disN = 0.0f;
	pos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);
	gPos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);
}

//デストラクタ
MenuScreen::~MenuScreen()
{
}

//更新
void MenuScreen::Update()
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP))
	{
		stageNum++;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN))
	{
		stageNum--;
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	//ステージ6からはパネルではなくカーソルを移動
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

	//左を押すと「戻る」にカーソルを移動
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LEFT))
	{
		cursorPos = Vector2(backPos.x + 300.0f, backPos.y);
	}
	else if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP)
		|| Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN))
	{
		cursorPos = Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y);
	}

	//ステージをクリアしたら次のステージを解放する
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) && stageNum != 8)
	{
		OpenNextStage();
	}

	//
	gPos = cursorPos;
	dir = gPos - pos;
	dis = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	//正規化
	disN = 1.0f / dis;
	dir.x *= disN;
	dir.y *= disN;
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::V))
	{
		gPos.y -= 150.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::B) && (pos.x != gPos.x || pos.y != gPos.y))
	{
		pos = pos + dir * 10.0f;
	}
}

//描画
void MenuScreen::Draw() const
{
	DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
	for (int i = 0; i < 9; i++)
	{
		std::string str;
		if (panel[i].isDraw == true) str = "true";
		else if (panel[i].isDraw == false) str = "false";
		DrawFormatString(0, 240 - i * 20, GetColor(255, 255, 255), "ステージ%d %s", i + 1, str);
	}

	//ステージパネルを描画
	for (int i = 0; i < 9; i++)
	{
		auto drawPos = panel[i].position + Vector2(0, 150 * panelNum);
		auto min = drawPos - Vector2(300.0f, 50.0f);
		auto max = drawPos + Vector2(300.0f, 50.0f);
		DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255 - 20 * i, 20 * i), panel[i].alpha);
	}
	//戻るパネルを描画
	DrawBox(backPos.x, backPos.y, backPos.x + 300.0f, backPos.y + 100.0f, GetColor(255, 0, 0), 1);

	//カーソルを描画
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, cursorPos, Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, false);

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M))
	{
		DrawFormatString(panel[cursorNum].position.x - 280, panel[cursorNum].position.y, GetColor(255, 0, 0), "ステージ%dを選択したよ！", stageNum + 1);
	}

	//
	DrawCircle(pos.x, pos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "pos %f %f", pos.x, pos.y);
}

//
void MenuScreen::Action()
{
}

//次のステージの解放
void MenuScreen::OpenNextStage()
{
	panel[stageNum + 1].isDraw = true;
	panel[stageNum + 1].alpha = 1.0f;
}