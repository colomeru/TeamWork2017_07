#include "MenuScreen.h"
#include "../../time/Time.h"
#include "../../math/MathHelper.h"
#include "../../graphic/Sprite.h"
#include "../../input/Keyboard.h"
#include "../../input/GamePad.h"
#include "../../math/Easing.h"

//�R���X�g���N�^
MenuScreen::MenuScreen()
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 0) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * 150.0f),false,0.0f };
	}
	backPos = Vector2(0.0f, WINDOW_HEIGHT - 100.0f);
	cursorPos = Vector2(panel[0].position.x - 350.0f, panel[0].position.y);
	backSelect = false;

	//
	dis = disN = 0.0f;
	pos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);
	gPos = Vector2(WINDOW_WIDTH / 4.0f, height - 50.0f);
}

//�f�X�g���N�^
MenuScreen::~MenuScreen()
{
}

//�X�V
void MenuScreen::Update()
{
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP)) &&
		backSelect == false) //�߂��I�����Ă��Ȃ����
	{
		stageNum++;
	}
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN)) &&
		backSelect == false) //�߂��I�����Ă��Ȃ����
	{
		stageNum--;
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
	dis = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	//���K��
	disN = 1.0f / dis;
	dir.x *= disN;
	dir.y *= disN;
	//Easing�v�Z
	float ease = Easing::EaseOutExpo(10.0f, pos.y, pos.y - gPos.y, 10.0f);

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::V))
	{
		gPos.y -= 150.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::B) && (pos.x != gPos.x || pos.y != gPos.y))
	{
		pos = pos + dir * ease;
	}
}

//�`��
void MenuScreen::Draw() const
{
	DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
	for (int i = 0; i < 9; i++)
	{
		std::string str;
		if (panel[i].isDraw == true) str = "true";
		else if (panel[i].isDraw == false) str = "false";
		DrawFormatString(0, 240 - i * 20, GetColor(255, 255, 255), "�X�e�[�W%d %c", i + 1, str);
	}

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
	DrawCircle(pos.x, pos.y, 16, GetColor(0, 0, 255), 0, 1);
	DrawCircle(gPos.x, gPos.y, 16, GetColor(255, 0, 0), 0, 1);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "pos %f %f", pos.x, pos.y);
}

//
void MenuScreen::Action()
{
}

//�O�̃X�e�[�W���N���A���Ă��邩�H
bool MenuScreen::CheckPreviousSrage(int sNum)
{
	if ((sNum > 0 && panel[sNum].isDraw == true) || sNum == 0) return true;
	else return false;
}

//���̃X�e�[�W�̉��
void MenuScreen::OpenNextStage(int sNum)
{
	if (CheckPreviousSrage(sNum) == true)
	{
		panel[sNum + 1].isDraw = true;
		panel[sNum + 1].alpha = 1.0f;
	}
}
