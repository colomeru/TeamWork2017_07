#include "InputMgr.h"
#include "DxLib.h"

#include "../Actor/Person/Player/State/Base/ActionType.h"

static const int MAX_BUTTON_NUM = 11;

InputMgr::InputMgr(){
	RegistKeyCode();
}

InputMgr::~InputMgr(){

}

InputMgr & InputMgr::GetInstance(){
	static InputMgr instance;
	return instance;
}

int InputMgr::GetHitKeyFrameAll()
{
	char GetHitKeyStateAll_Key[256];
	GetHitKeyStateAll(GetHitKeyStateAll_Key);

	for (int i = 0; i < MAX_KEY_NUM; i++) {
		previous_key_state[i] = current_key_state[i];

		if (GetHitKeyStateAll_Key[i] != 0) {
			current_key_state[i]++;
		}
		else current_key_state[i] = 0;
	}
	previous_button_state = current_button_state;
	current_button_state = GetJoypadInputState(DX_INPUT_PAD1);

	return 0;
}

void InputMgr::setButton(Buttons targetButton,unsigned int changeButton)
{
	ButtonName[targetButton] = changeButton;

	//if ((current_button_state&PAD_INPUT_1) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_2) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_3) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_4) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_5) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_6) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_7) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_8) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_9) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_10) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_11) != 0)ButtonName[targetButton] = PAD_INPUT_1;
	//if ((current_button_state&PAD_INPUT_12) != 0)ButtonName[targetButton] = PAD_INPUT_1;	
}

void InputMgr::RegistKeyCode(){
	KeyName[KeyCode::UP]	 = KEY_INPUT_UP;
	KeyName[KeyCode::DOWN]	 = KEY_INPUT_DOWN;
	KeyName[KeyCode::RIGHT]  = KEY_INPUT_RIGHT;
	KeyName[KeyCode::LEFT]	 = KEY_INPUT_LEFT;
	KeyName[KeyCode::ESCAPE] = KEY_INPUT_ESCAPE;
	KeyName[KeyCode::SPACE]  = KEY_INPUT_SPACE;
	KeyName[KeyCode::RETURN] = KEY_INPUT_RETURN;
	KeyName[KeyCode::A] = KEY_INPUT_A;
	KeyName[KeyCode::B] = KEY_INPUT_B;
	KeyName[KeyCode::C] = KEY_INPUT_C;
	KeyName[KeyCode::D] = KEY_INPUT_D;
	KeyName[KeyCode::E] = KEY_INPUT_E;
	KeyName[KeyCode::F] = KEY_INPUT_F;
	KeyName[KeyCode::G] = KEY_INPUT_G;
	KeyName[KeyCode::H] = KEY_INPUT_H;
	KeyName[KeyCode::I] = KEY_INPUT_I;
	KeyName[KeyCode::J] = KEY_INPUT_J;
	KeyName[KeyCode::K] = KEY_INPUT_K;
	KeyName[KeyCode::L] = KEY_INPUT_L;
	KeyName[KeyCode::M] = KEY_INPUT_M;
	KeyName[KeyCode::N] = KEY_INPUT_N;
	KeyName[KeyCode::O] = KEY_INPUT_O;
	KeyName[KeyCode::P] = KEY_INPUT_P;
	KeyName[KeyCode::Q] = KEY_INPUT_Q;
	KeyName[KeyCode::R] = KEY_INPUT_R;
	KeyName[KeyCode::S] = KEY_INPUT_S;
	KeyName[KeyCode::T] = KEY_INPUT_T;
	KeyName[KeyCode::U] = KEY_INPUT_U;
	KeyName[KeyCode::V] = KEY_INPUT_V;
	KeyName[KeyCode::W] = KEY_INPUT_W;
	KeyName[KeyCode::X] = KEY_INPUT_X;
	KeyName[KeyCode::Y] = KEY_INPUT_Y;
	KeyName[KeyCode::Z] = KEY_INPUT_Z;
	KeyName[KeyCode::L_SHIFT] = KEY_INPUT_LSHIFT;
	KeyName[KeyCode::R_SHIFT] = KEY_INPUT_RSHIFT;

	ButtonName[Buttons::BUTTON_CIRCLE] = PAD_INPUT_2;
	ButtonName[Buttons::BUTTON_CROSS] = PAD_INPUT_3;
	ButtonName[Buttons::BUTTON_START] = PAD_INPUT_9;
	ButtonName[Buttons::BUTTON_SELECT] = PAD_INPUT_10;
	ButtonName[Buttons::BUTTON_L1] = PAD_INPUT_7;
	ButtonName[Buttons::BUTTON_R1] = PAD_INPUT_8;
	ButtonName[Buttons::BUTTON_L2] = PAD_INPUT_5;
	ButtonName[Buttons::BUTTON_R2] = PAD_INPUT_6;
	ButtonName[Buttons::BUTTON_UP] = PAD_INPUT_UP;
	ButtonName[Buttons::BUTTON_DOWN] = PAD_INPUT_DOWN;
	ButtonName[Buttons::BUTTON_RIGHT] = PAD_INPUT_RIGHT;
	ButtonName[Buttons::BUTTON_LEFT] = PAD_INPUT_LEFT;
	//ボタンとキーボードの関連付け
	ConnectInputName[BUTTON_L1] = KeyCode::L_SHIFT;
	ConnectInputName[BUTTON_R1] = KeyCode::R_SHIFT;
	ConnectInputName[BUTTON_L2] = KeyCode::Q;
	ConnectInputName[BUTTON_R2] = KeyCode::L;
	ConnectInputName[BUTTON_CIRCLE] = KeyCode::SPACE;
	ConnectInputName[BUTTON_CROSS] = KeyCode::X;
	ConnectInputName[BUTTON_START] = KeyCode::T;
	ConnectInputName[BUTTON_UP] = KeyCode::UP;
	ConnectInputName[BUTTON_DOWN] = KeyCode::DOWN;
	ConnectInputName[BUTTON_RIGHT] = KeyCode::RIGHT;
	ConnectInputName[BUTTON_LEFT] = KeyCode::LEFT;
}
bool InputMgr::IsPushButton(){
	for (auto i : current_key_state) {
		if(i != 0)return true;
	}
	for (auto i : ButtonName) {
		if (i.first == BUTTON_UP || i.first == BUTTON_DOWN || i.first == BUTTON_RIGHT || i.first == BUTTON_LEFT)continue;

		if ((current_button_state&i.second) != 0)return true;
	}
	return false;
}

bool InputMgr::IsKeyDown(KeyCode handle)
{
	if (previous_key_state[KeyName[handle]] == 0 && current_key_state[KeyName[handle]] == 1)return true;
	return false;
}

bool InputMgr::IsKeyOn(KeyCode handle)
{
	if (previous_key_state[KeyName[handle]] != 0 && current_key_state[KeyName[handle]] != 0)return true;
	return false;
}

bool InputMgr::IsKeyUp(KeyCode handle)
{
	if (previous_key_state[KeyName[handle]] != 0 && current_key_state[KeyName[handle]] == 0)return true;
	return false;
}

bool InputMgr::IsButtonDown(Buttons handle)
{
	if ((previous_button_state&ButtonName[handle])==0 &&(current_button_state&ButtonName[handle]) != 0) {
		return true;
	}
	else {
		return IsKeyDown(ConnectInputName[handle]);
	}
}

bool InputMgr::IsButtonOn(Buttons handle)
{
	if ((previous_button_state & ButtonName[handle]) != 0 && (current_button_state & ButtonName[handle]) != 0) {
		return true;
	}
	else {
		return IsKeyOn(ConnectInputName[handle]);
	}
}

bool InputMgr::IsButtonUp(Buttons handle)
{
	if ((previous_button_state & ButtonName[handle]) != 0 && (current_button_state & ButtonName[handle]) == 0) {
		return true;
	}
	else {
		return IsKeyUp(ConnectInputName[handle]);
	}
}

bool InputMgr::isConnectGamePad()
{
	if (GetJoypadNum() != 0) {
		return true;
	}
	return false;
}

Vector2 InputMgr::AnalogPadVectorL(){
	int horizontal;
	int vertical;
	GetJoypadAnalogInput(&horizontal, &vertical, DX_INPUT_PAD1);
	Vector2 input = Vector2(static_cast<float>(horizontal), static_cast<float>(vertical)) / 1000.0f;
	if (input.LengthSquared() > 0) return input;
	return KeyVectorL();
}

Vector2 InputMgr::AnalogPadVectorR(){
	DINPUT_JOYSTATE joy;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &joy);

	int horizontal;
	int vertical;

	horizontal = joy.Z;
	vertical = joy.Rz;

	Vector2 input = Vector2(static_cast<float>(horizontal), static_cast<float>(vertical)) / 1000.0f;
	if (input.LengthSquared() > 0) return input;
	return KeyVectorR();
}

Vector2 InputMgr::DirectPadVector() {
	DINPUT_JOYSTATE joy;
	GetJoypadDirectInputState(DX_INPUT_PAD1,&joy);

	if (joy.POV[0] == 0xffffffff) return Vector2::Zero;

	float horizontal=0;
	float vertical=0;

	if (joy.POV[0] == 0) {
		vertical = -1;

	}
	if (joy.POV[0] == 4500) {
		vertical = -1;
		horizontal = 1;
	}
	if (joy.POV[0] == 9000) {
		horizontal = 1;
	}
	if (joy.POV[0] == 4500) {
		vertical = -1;
		horizontal = 1;
	}
	if (joy.POV[0] == 13500) {
		vertical = 1;
		horizontal = 1;
	}
	if (joy.POV[0] == 18000) {
		vertical = 1;
	}
	if (joy.POV[0] == 22500) {
		vertical = 1;
		horizontal = -1;
	}
	if (joy.POV[0] == 27000) {
		horizontal = -1;
	}
	if (joy.POV[0] == 31500) {
		vertical = -1;
		horizontal = -1;
	}
	//DrawFormatString(0, 48, GetColor(255,255,255), "POV 0:%d 1:%d 2:%d 3:%d",
	//	joy.POV[0], joy.POV[1],
	//	joy.POV[2], joy.POV[3]);

	return Vector2(horizontal*1000 ,vertical*1000);
}

Vector2 InputMgr::XPadVector()
{
	XINPUT_STATE joy;
	GetJoypadXInputState(DX_INPUT_PAD1,&joy);
	float horizontal= joy.ThumbLX;
	float vertical = joy.ThumbLY;
	return Vector2(horizontal, vertical);
}



Vector2 InputMgr::KeyVector(KeyCode right, KeyCode left, KeyCode up, KeyCode down){
	Vector2 input = Vector2::Zero;
	if (IsKeyOn(right) && IsKeyOn(left))input.x = 0;
	else if (IsKeyOn(right))input.x = 1;
	else if (IsKeyOn(left))	input.x = -1;
	if (IsKeyOn(up) && IsKeyOn(down))input.y = 0;
	else if (IsKeyOn(up))	input.y = -1;
	else if (IsKeyOn(down))	input.y = 1;
	return input;
}

// 右用
Vector2 InputMgr::KeyVectorL(KeyCode right, KeyCode left, KeyCode up, KeyCode down) {
	return KeyVector(right, left, up, down);
}

// 左用
Vector2 InputMgr::KeyVectorR(KeyCode right, KeyCode left, KeyCode up, KeyCode down){
	return KeyVector(right, left, up, down);
}

Vector2 InputMgr::KeyVector(const ActionType & type){
	if (type == ActionType::Right) return KeyVectorR();
	if (type == ActionType::Left)  return KeyVectorL();
	return Vector2::Zero;
}

Vector2 InputMgr::PadVector(const ActionType & type) {
	if (type == ActionType::Right) return AnalogPadVectorR();
	if (type == ActionType::Left)  return AnalogPadVectorL();
	return Vector2::Zero;
}



