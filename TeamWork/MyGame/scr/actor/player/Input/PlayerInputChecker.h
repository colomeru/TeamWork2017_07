#pragma once
#include"../../../input/InputChecker.h"

class PlayerInputChecker {
public:
	PlayerInputChecker(bool isUse);

	//キーが押されたかを返す
	bool KeyTriggerDown(InputChecker::Input_Key key)const;
	//キーが離したかを返す
	bool KeyTriggerUp(InputChecker::Input_Key key)const;
	//キーを押しているかを返す
	bool KeyStateDown(InputChecker::Input_Key key)const;
	//キーが離されているかを返す
	bool KeyStateUp(InputChecker::Input_Key key)const;

	//スティックの傾きを返す
	Vector2 Stick()const;

	//指定方向にスティックが倒れているかを返す
	bool StickStateDown(InputChecker::Input_Stick stick)const;
	//指定方向にスティックを倒したかを返す
	bool StickTriggerDown(InputChecker::Input_Stick stick)const;

	//キーを利用可能かを受け取る
	bool GetUseKey()const;
	//キーを利用可能かを設定する
	void SetUseKey(bool isUse);
private:
	bool useKey_;
};