#include "Keyboard.h"

// コンストラクタ
Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++)
	{
		m_onkey[i]  = 0;
		m_offkey[i] = 0;
	}
}

// キーを押した時のトリガー判定
bool Keyboard::KeyTriggerDown(UINT keycode)
{
	if (m_onkey[keycode] == 1)	return true;
	return false;
}

// キーを押した時のステート判定
bool Keyboard::KeyStateDown(UINT keycode)
{
	if (m_keycode[keycode] == 1)	return true;
	return false;
}

// キーを離した時のトリガー判定
bool Keyboard::KeyTriggerUp(UINT keycode)
{
	if (m_offkey[keycode] == 1)	return true;
	return false;
}

// キーを離した時のステート判定
bool Keyboard::KeyStateUp(UINT keycode)
{
	if (m_keycode[keycode] == 0)	return true;
	return false;
}

// いずれかのキーが入力されたか瞬間を調べる
bool Keyboard::AnyTriggerDown()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_onkey[i] == 1)return true;
	}
	return false;
}

// いずれかのキーが入力されているか調べる
bool Keyboard::AnyStateDown()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_keycode[i] == 1)return true;
	}
	return false;
}

// いずれかのキーが離れたか瞬間を調べる
bool Keyboard::AnyTriggerUp()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_offkey[i] == 1)	return true;
	}
	return false;
}

// いずれかのキーが離れているか調べる
bool Keyboard::AnyStateUp()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_keycode[i] == 0)	return true;
	}
	return false;
}

// 更新処理
void Keyboard::Update()
{
	GetHitKeyStateAll(m_keycode);

	for (int i = 0; i < 256; i++)
	{
		UpdateKey(m_keycode[i], i);
	}
}

void Keyboard::UpdateKey(char key, int num)
{
	// 押されていれば
	if (key != 0)
	{
		m_offkey[num] = 0;
		m_onkey[num]++;
	}
	// 押されていなければ
	else
	{
		m_onkey[num] = 0;
		m_offkey[num]++;
	}
}
