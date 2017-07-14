#pragma once
#include"Keyboard.h"
#include"GamePad.h"
#include<map>

class InputChecker {
public:
	enum Input_Key {
		A,
		B,
		X,
		R1,
		L1,
		Start,
		Back,
	};
	enum Input_Stick {
		Up,
		Down,
		Left,
		Right
	};
private:
	InputChecker();
public:
	static InputChecker& GetInstance() {
		static InputChecker ic;
		return ic;
	}
	//キーが押されたかを返す
	bool KeyTriggerDown(Input_Key key)const;
	//キーが離したかを返す
	bool KeyTriggerUp(Input_Key key)const;
	//キーを押しているかを返す
	bool KeyStateDown(Input_Key key)const;
	//キーが離されているかを返す
	bool KeyStateUp(Input_Key key)const;

	//スティックの傾きを返す
	Vector2 Stick()const;

	//指定方向にスティックが倒れているかを返す
	bool StickStateDown(Input_Stick stick)const;
	//指定方向にスティックを倒したかを返す
	bool StickTriggerDown(Input_Stick stick)const;
private:
	using Connect_Key_Pad = std::pair<UINT, UINT>;
	std::map<Input_Key, Connect_Key_Pad> inputList_;

};