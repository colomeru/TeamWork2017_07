#include "DxLib.h"
#include "game\Game1.h"
#include "AllInclude.h"
#include "scene\SceneManager.h"

#define	_CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game1 game;

	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		game.Run();
	}
	catch (const std::string& error)
	{
		MessageBox(nullptr, error.c_str(), "エラー", MB_OK);
		game.Finalize();
		DxLib_End();
	}
	catch (std::runtime_error& e)	/* 何かしら例外が吐かれていたら */
	{
		MessageBox(nullptr, e.what(), "エラー", MB_OK);
		game.Finalize();
		DxLib_End();
	}

	return 0;					// ソフトの終了
}