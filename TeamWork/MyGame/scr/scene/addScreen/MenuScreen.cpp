#include "MenuScreen.h"
#include "../../time/Time.h"
#include "../../math/MathHelper.h"
#include "../../graphic/Sprite.h"
#include "../../input/Keyboard.h"
#include "../../input/GamePad.h"
#include "../../math/Easing.h"
#include "../../tween/TweenManager.h"
#include "../../Def.h"

const Vector2 CursorPos[2]{ Vector2(WINDOW_WIDTH / 2.0f - 350.0f, WINDOW_HEIGHT / 2.0f),
							Vector2(0.0f, WINDOW_HEIGHT - 100.0f) };

//コンストラクタ
MenuScreen::MenuScreen() :stageNum(0)
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 0) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),false,0.0f };
	}
	//本来は0版はチュートリアル
	stageList_[0] = Stage::Stage1;
	stageList_[1] = Stage::Stage1;
	stageList_[2] = Stage::Stage2;
	stageList_[3] = Stage::Stage3;
	stageList_[4] = Stage::Stage4;
	stageList_[5] = Stage::Stage5;
	stageList_[6] = Stage::Stage6;
	stageList_[7] = Stage::Stage7;
	stageList_[8] = Stage::Stage8;

	//backPos = Vector2(0.0f, WINDOW_HEIGHT - 100.0f);
	//cursorPos = Vector2(panel[0].position.x - 350.0f, panel[0].position.y);
	cursorPos = CursorPos[0];
	backSelect = false;

	//
	dis = disN = 0.0f;
	pos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);
	gPos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);

	timer_ = 0.0f;
	from = 0.0f;
	ease = 0.0f;
	moveDis = Vector2(0.0f, 0.0f);
	velocity = Vector2(0.0f, 0.0f);
	modify = Vector2(0.0f, 0.0f);
	mag = 50.0f;
}

//デストラクタ
MenuScreen::~MenuScreen()
{
}

//更新
void MenuScreen::Update()
{
	//Pattern1Update();
	Pattern2Update();
}

//描画
void MenuScreen::Draw() const
{
	//Pattern1Draw();
	Pattern2Draw();
}

//
void MenuScreen::Action()
{
}

//前のステージをクリアしているか？
bool MenuScreen::CheckPreviousStage(int sNum)
{
	if ((sNum > 0 && panel[sNum].isDraw == true) || sNum == 0) return true;
	else return false;
}

//次のステージの解放
void MenuScreen::OpenNextStage(int sNum)
{
	if (CheckPreviousStage(sNum) == true)
	{
		panel[sNum + 1].isDraw = true;
		panel[sNum + 1].alpha = 1.0f;
	}
}

//次のステージが解放されているか？
bool MenuScreen::CheckNextStage(int sNum)
{
	if (sNum < 8 && panel[sNum + 1].isDraw == true) return true;
	else return false;
}

//パターン１更新
void MenuScreen::Pattern1Update()
{
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP)) &&
		backSelect == false) //戻るを選択していなければ
	{
		stageNum++;
		timer_ = 0.0f;
		dis = -150.0f;
	}
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN)) &&
		backSelect == false) //戻るを選択していなければ
	{
		stageNum--;
		timer_ = 0.0f;
		dis = 150.0f;
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	//ステージ6からはパネルではなくカーソルを移動
	if (stageNum < 5)
	{ //ステージ5までは
		panelNum = stageNum;
		cursorNum = 0;
	}
	else
	{ //ステージ6からは
		panelNum = 4;
		cursorNum = stageNum - 4;
	}

	//左を押すと「戻る」にカーソルを移動
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LEFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::LEFT))
	{
		cursorPos = Vector2(backPos.x + 300.0f, backPos.y);
		backSelect = true;
	}
	else if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN))
	{
		cursorPos = Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y);
		backSelect = false;
	}

	//ステージをクリアしたら次のステージを解放する
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || //AボタンかMを押すとステージクリア（仮）
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) &&
		stageNum != 8 && //ステージ9以外
		backSelect == false) //戻るを選択していなければ
	{
		OpenNextStage(stageNum);
	}

	//カーソルの位置から目標位置へのベクトルを求める
	gPos = cursorPos;
	dir = gPos - pos;
	//正規化
	dir = Vector2(dir.x, dir.y).Normalize();
	//Easing計算


	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::V))
	{
		gPos.y -= 150.0f;
	}
	timer_ += Time::DeltaTime;
	timer_ = MathHelper::Clamp(timer_, 0.0f, 10.0f);
	float ease = Easing::EaseOutExpo(timer_, pos.y, gPos.y - pos.y, 2.0f);

	//timer_ = 0.0f;
	drawPos.y = ease;
	drawPos.x = pos.x;
}

//パターン１描画
void MenuScreen::Pattern1Draw() const
{
	DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "pos %f %f", pos.x, pos.y);

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
		DrawFormatString(panel[cursorNum].position.x - 280, panel[cursorNum].position.y, GetColor(255, 0, 0), "ステージ%d", stageNum + 1);
	}

	//
	DrawCircle(drawPos.x, drawPos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
}

//パターン２更新
void MenuScreen::Pattern2Update()
{
	if (!backSelect) {
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
			GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP))
		{
			if (stageNum == 8 || CheckNextStage(stageNum) == false) return;
			timer_ = 0.0f;
			dis += 150.0f;
			//dis = drawPos.y - panel[stageNum].position.y;
			stageNum++;
			//if (dis < 0) return;
			dir = Vector2(0.0f, 1.0f);

			TweenManager::GetInstance().Add(EaseOutExpo, &from, Vector2(0.0f, dis), MoveTime);
		}
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
			GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN))
		{
			if (stageNum == 0.0f) return;
			timer_ = 0.0f;
			dis -= 150.0f;
			//dis = panel[stageNum].position.y - drawPos.y;
			stageNum--;
			//if (dis > 0.0f) return;
			dir = Vector2(0.0f, -1.0f);

			TweenManager::GetInstance().Add(EaseOutExpo, &from, Vector2(0.0f, dis), MoveTime);
		}
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	panelNum = stageNum;
	cursorNum = 0;

	//左を押すと「戻る」にカーソルを移動
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LEFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::LEFT))
	{
		//cursorPos = Vector2(backPos.x + 32.0f, backPos.y);
		//backPos = Vector2(0.0f, WINDOW_HEIGHT - 100.0f);
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos, CursorPos[1], MoveTime);
		backSelect = true;
	}
	else if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RIGHT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::RIGHT))
	{
		//backPos = Vector2(WINDOW_WIDTH / 2.0f - 350.0f, WINDOW_HEIGHT / 2.0f);
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos, CursorPos[0], MoveTime);
		backSelect = false;
	}

	//ステージをクリアしたら次のステージを解放する
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || //AボタンかMを押すとステージクリア（仮）
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) &&
		stageNum != 8 && //ステージ9以外
		backSelect == false) //戻るを選択していなければ
	{
		OpenNextStage(stageNum);
	}

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::V))
	{
		gPos.y += 150.0f;
	}
	timer_ += Time::DeltaTime;
	timer_ = MathHelper::Min(timer_, MoveTime);

	//ease = Easing::EaseOutExpo(timer_, from, dis, MoveTime);

	//if ((moveDis.y > 0.0f && moveDis.y > dis - 100.0f) || (moveDis.y < 0.0f && moveDis.y < dis + 100.0f)) mag = 5.0f;
	//else mag = 30.0f;
	//mag = 50.0f;
	//velocity.y = dir.y * mag;
	//if ((dis > 0 && moveDis.y < dis) || (dis < 0 && moveDis.y > dis)) {
	//	//if (moveDis.y != dis) {
	//	moveDis += velocity;
	//	modify += velocity;
	//}
	//else {
	//	moveDis.y = 0.0f;
	//	dis = 0.0f;
	//	velocity.y = 0.0f;
	//}
	MathHelper::Clamp(modify.y, 0.0f, 1200.0f);
}

//パターン２描画
void MenuScreen::Pattern2Draw() const
{
	if (BuildMode == 1) {
		DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
		DrawFormatString(0, 60, GetColor(255, 255, 255), "pos %f %f", pos.x, pos.y);
		DrawFormatString(0, 80, GetColor(255, 255, 255), "ease %f", ease);
		DrawFormatString(0, 100, GetColor(255, 255, 255), "dis %f", dis);
		DrawFormatString(0, 120, GetColor(255, 255, 255), "velocity %f %f", velocity.x, velocity.y);
		DrawFormatString(0, 140, GetColor(255, 255, 255), "meveDis %f %f", moveDis.x, moveDis.y);
		DrawFormatString(0, 160, GetColor(255, 255, 255), "modify %f %f", modify.x, modify.y);
		DrawFormatString(0, 180, GetColor(255, 255, 255), "mag %f", mag);
	}
	//ステージパネルを描画
	for (int i = 0; i < 9; i++)
	{
		//if (!test) return;
		auto drawPos = panel[i].position + from;

		//float ease = Easing::EaseOutExpo(timer_, 0.0f, dis, 1.0f);
		//drawPos.y += ease;
		//if ((stageNum != 0 && dis == -150.0f) || (stageNum != 8 && dis == 150.0f)) drawPos.y += ease;
		auto min = drawPos - Vector2(300.0f, 50.0f);
		auto max = drawPos + Vector2(300.0f, 50.0f);
		DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255 - 20 * i, 20 * i), panel[i].alpha);
	}
	//0.0f, WINDOW_HEIGHT - 100.0f

	//戻るパネルを描画
	DrawBox(CursorPos[1].x, CursorPos[1].y, CursorPos[1].x + 300.0f, CursorPos[1].y + 100.0f, GetColor(255, 0, 0), 1);

	//カーソルを描画
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, cursorPos, Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, false);

	//Vector2 cursorPos2;
	//if (!backSelect) cursorPos2 = Vector2(cursorPos.x + 700.0f,cursorPos.y);
	//else cursorPos2 = Vector2(cursorPos.x + 400.0f, cursorPos.y);
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(cursorPos2.x, cursorPos2.y), Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, true);

	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M))
	//{
	//	DrawFormatString(panel[cursorNum].position.x - 280, panel[cursorNum].position.y, GetColor(255, 0, 0), "ステージ%d", stageNum + 1);
	//}

	//
	DrawCircle(drawPos.x, drawPos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
}

Stage MenuScreen::GetGamePlayStage() const
{
	return stageList_[stageNum];
}
