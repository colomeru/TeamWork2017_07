#pragma once

#include "DxLib.h"
#include "DxDirectX.h"
#include <array>

namespace KEYCODE
{
	const UINT RIGHT = D_DIK_RIGHTARROW;
	const UINT LEFT = D_DIK_LEFTARROW;
	const UINT UP = D_DIK_UPARROW;
	const UINT DOWN = D_DIK_DOWNARROW;
	const UINT A = D_DIK_A;
	const UINT B = D_DIK_B;
	const UINT C = D_DIK_C;
	const UINT D = D_DIK_D;
	const UINT E = D_DIK_E;
	const UINT F = D_DIK_F;
	const UINT G = D_DIK_G;
	const UINT H = D_DIK_H;
	const UINT I = D_DIK_I;
	const UINT J = D_DIK_J;
	const UINT K = D_DIK_K;
	const UINT L = D_DIK_L;
	const UINT M = D_DIK_M;
	const UINT N = D_DIK_N;
	const UINT O = D_DIK_O;
	const UINT P = D_DIK_P;
	const UINT Q = D_DIK_Q;
	const UINT R = D_DIK_R;
	const UINT S = D_DIK_S;
	const UINT T = D_DIK_T;
	const UINT U = D_DIK_U;
	const UINT V = D_DIK_V;
	const UINT W = D_DIK_W;
	const UINT X = D_DIK_X;
	const UINT Y = D_DIK_Y;
	const UINT Z = D_DIK_Z;
	const UINT LSHIFT = D_DIK_LSHIFT;
	const UINT LCTRL = D_DIK_LCONTROL;
	const UINT RSHIFT = D_DIK_RSHIFT;
	const UINT RCTRL = D_DIK_RCONTROL;
	const UINT SPACE = D_DIK_SPACE;
	const UINT NUM1 = D_DIK_1;
	const UINT NUM2 = D_DIK_2;
	const UINT NUM3 = D_DIK_3;
	const UINT NUM4 = D_DIK_4;
	const UINT NUM5 = D_DIK_5;
	const UINT NUM6 = D_DIK_6;
	const UINT NUM7 = D_DIK_7;
	const UINT NUM8 = D_DIK_8;
	const UINT NUM9 = D_DIK_9;
	const UINT NUM0 = D_DIK_0;
	const UINT TAB = D_DIK_TAB;
	const UINT ESC = D_DIK_ESCAPE;
}

class Keyboard
{
private:
	Keyboard();
	~Keyboard() = default;

public:
	static Keyboard &GetInstance() {
		static Keyboard k;
		return k;
	}

	/// <summary>
	/// 指定されたキーが入力された瞬間か調べる
	/// </summary>
	bool KeyTriggerDown(UINT keycode);
	/// <summary>
	/// 指定されたキーが入力されているか調べる
	/// </summary>
	bool KeyStateDown(UINT keycode);
	/// <summary>
	/// 指定されたキーが離れた瞬間か調べる
	/// </summary>
	bool KeyTriggerUp(UINT keycode);
	/// <summary>
	/// 指定されたキーが離れているか調べる
	/// </summary>
	bool KeyStateUp(UINT keycode);
	/// <summary>
	/// いずれかのキーが入力された瞬間か調べる
	/// </summary>
	bool AnyTriggerDown();
	/// <summary>
	/// いずれかのキーが入力されているか調べる
	/// </summary>
	bool AnyStateDown();
	/// <summary>
	/// いずれかのキーが離れた瞬間か調べる
	/// </summary>
	bool AnyTriggerUp();
	/// <summary>
	/// いずれかのキーが離れているか調べる
	/// </summary>
	bool AnyStateUp();

	void Update();
private:
	void UpdateKey(char key, int num);
	char m_keycode[256];
	std::array<int, 256> m_onkey;
	std::array<int, 256> m_offkey;
};