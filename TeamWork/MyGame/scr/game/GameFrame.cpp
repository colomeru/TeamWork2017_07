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

	ChangeWindowMode(TRUE);		// �E�B���h�E���[�h

	if (DxLib_Init() == -1)		// DX���C�u��������������
	{
		// �G���[���N�����璼���ɏI��
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
		//ScreenFlip���Ă΂�Ă���̂ł���Ȃ�
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
