#include "GameDevice.h"
#include "../Define.h"
#include "../resource.h"

GameDevice::GameDevice(int width, int height, float rate, int windowFlag, float fps, int colorBitDepth){
	SetWindowIconID(IDI_MYICON); 
	SetMainWindowText("Twin Slime");
	SetGraphMode(width, height, colorBitDepth);
	ChangeWindowMode(windowFlag);
	SetWindowSizeExtendRate(rate);
	//SetBackgroundColor(0, 0, 0);

	//DXLib初期化処理
	if (DxLib_Init() == -1)DxLib_End();//エラーが起きたら終了

	SetDrawScreen(DX_SCREEN_BACK);
}

GameDevice::~GameDevice(){}

void GameDevice::flip(){
	ScreenFlip();
}
