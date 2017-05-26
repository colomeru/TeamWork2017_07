#include "GameFrame.h"
#include <DxLib.h>
#include "../Def.h"

bool GameFrame::mIsEnd = false;

void GameFrame::Run()
{
	SetAlwaysRunFlag(TRUE);
	SetUseBackCulling(TRUE);
	SetLogDrawOutFlag(FALSE);
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BIT_DEPTH, REFRESH_RATE);
	SetWindowSizeExtendRate(0.7f, 0.7f);
	SetBackgroundColor(153, 204, 255);
	SetMainWindowText(GAMENAME);
	SetZBufferBitDepth(24);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetWaitVSyncFlag(TRUE);

	ChangeWindowMode(TRUE);		// ウィンドウモード

	if (DxLib_Init() == -1)		// DXライブラリ初期化処理
	{
		// エラーが起きたら直ちに終了
		DxLib_End();
		return;					
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Initialize();

	RECT screen = { 0,0,WINDOW_WIDTH, WINDOW_HEIGHT };

	while (ProcessMessage() == 0 && !IsEnd())
	{
		Update();
		fps.Update();

		ClearDrawScreen(&screen);
		ClearDrawScreenZBuffer(&screen);

		Draw();

		ScreenFlip();
		//ScreenFlipが呼ばれているのでいらない
		//fps.Wait();
	}

	Finalize();
	DxLib_End();
}

bool GameFrame::IsEnd()
{
	return mIsEnd;
}

void GameFrame::GameEnd()
{
	mIsEnd = true;
}
