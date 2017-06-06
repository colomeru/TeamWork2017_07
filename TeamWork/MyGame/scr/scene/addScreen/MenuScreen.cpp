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

//�R���X�g���N�^
MenuScreen::MenuScreen() :stageNum(0)
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 0) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),false,0.0f };
	}
	//�{����0�ł̓`���[�g���A��
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

//�f�X�g���N�^
MenuScreen::~MenuScreen()
{
}

//�X�V
void MenuScreen::Update()
{
	//Pattern1Update();
	Pattern2Update();
}

//�`��
void MenuScreen::Draw() const
{
	//Pattern1Draw();
	Pattern2Draw();
}

//
void MenuScreen::Action()
{
}

//�O�̃X�e�[�W���N���A���Ă��邩�H
bool MenuScreen::CheckPreviousStage(int sNum)
{
	if ((sNum > 0 && panel[sNum].isDraw == true) || sNum == 0) return true;
	else return false;
}

//���̃X�e�[�W�̉��
void MenuScreen::OpenNextStage(int sNum)
{
	if (CheckPreviousStage(sNum) == true)
	{
		panel[sNum + 1].isDraw = true;
		panel[sNum + 1].alpha = 1.0f;
	}
}

//���̃X�e�[�W���������Ă��邩�H
bool MenuScreen::CheckNextStage(int sNum)
{
	if (sNum < 8 && panel[sNum + 1].isDraw == true) return true;
	else return false;
}

//�p�^�[���P�X�V
void MenuScreen::Pattern1Update()
{
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP)) &&
		backSelect == false) //�߂��I�����Ă��Ȃ����
	{
		stageNum++;
		timer_ = 0.0f;
		dis = -150.0f;
	}
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN)) &&
		backSelect == false) //�߂��I�����Ă��Ȃ����
	{
		stageNum--;
		timer_ = 0.0f;
		dis = 150.0f;
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	//�X�e�[�W6����̓p�l���ł͂Ȃ��J�[�\�����ړ�
	if (stageNum < 5)
	{ //�X�e�[�W5�܂ł�
		panelNum = stageNum;
		cursorNum = 0;
	}
	else
	{ //�X�e�[�W6�����
		panelNum = 4;
		cursorNum = stageNum - 4;
	}

	//���������Ɓu�߂�v�ɃJ�[�\�����ړ�
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

	//�X�e�[�W���N���A�����玟�̃X�e�[�W���������
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || //A�{�^����M�������ƃX�e�[�W�N���A�i���j
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) &&
		stageNum != 8 && //�X�e�[�W9�ȊO
		backSelect == false) //�߂��I�����Ă��Ȃ����
	{
		OpenNextStage(stageNum);
	}

	//�J�[�\���̈ʒu����ڕW�ʒu�ւ̃x�N�g�������߂�
	gPos = cursorPos;
	dir = gPos - pos;
	//���K��
	dir = Vector2(dir.x, dir.y).Normalize();
	//Easing�v�Z


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

//�p�^�[���P�`��
void MenuScreen::Pattern1Draw() const
{
	DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "pos %f %f", pos.x, pos.y);

	//�X�e�[�W�p�l����`��
	for (int i = 0; i < 9; i++)
	{
		auto drawPos = panel[i].position + Vector2(0, 150 * panelNum);
		auto min = drawPos - Vector2(300.0f, 50.0f);
		auto max = drawPos + Vector2(300.0f, 50.0f);
		DrawBox(min.x, min.y, max.x, max.y, GetColor(0, 255 - 20 * i, 20 * i), panel[i].alpha);
	}
	//�߂�p�l����`��
	DrawBox(backPos.x, backPos.y, backPos.x + 300.0f, backPos.y + 100.0f, GetColor(255, 0, 0), 1);

	//�J�[�\����`��
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, cursorPos, Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, false);

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M))
	{
		DrawFormatString(panel[cursorNum].position.x - 280, panel[cursorNum].position.y, GetColor(255, 0, 0), "�X�e�[�W%d", stageNum + 1);
	}

	//
	DrawCircle(drawPos.x, drawPos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
}

//�p�^�[���Q�X�V
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

	//���������Ɓu�߂�v�ɃJ�[�\�����ړ�
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

	//�X�e�[�W���N���A�����玟�̃X�e�[�W���������
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || //A�{�^����M�������ƃX�e�[�W�N���A�i���j
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) &&
		stageNum != 8 && //�X�e�[�W9�ȊO
		backSelect == false) //�߂��I�����Ă��Ȃ����
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

//�p�^�[���Q�`��
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
	//�X�e�[�W�p�l����`��
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

	//�߂�p�l����`��
	DrawBox(CursorPos[1].x, CursorPos[1].y, CursorPos[1].x + 300.0f, CursorPos[1].y + 100.0f, GetColor(255, 0, 0), 1);

	//�J�[�\����`��
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(panel[cursorNum].position.x - 350.0f, panel[cursorNum].position.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, cursorPos, Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, false);

	//Vector2 cursorPos2;
	//if (!backSelect) cursorPos2 = Vector2(cursorPos.x + 700.0f,cursorPos.y);
	//else cursorPos2 = Vector2(cursorPos.x + 400.0f, cursorPos.y);
	//Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(cursorPos2.x, cursorPos2.y), Vector2(32.0f, 32.0f), 1.0f, Vector2::One, true, true);

	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M))
	//{
	//	DrawFormatString(panel[cursorNum].position.x - 280, panel[cursorNum].position.y, GetColor(255, 0, 0), "�X�e�[�W%d", stageNum + 1);
	//}

	//
	DrawCircle(drawPos.x, drawPos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
}

Stage MenuScreen::GetGamePlayStage() const
{
	return stageList_[stageNum];
}
