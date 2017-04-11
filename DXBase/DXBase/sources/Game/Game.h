#pragma once

#include "DxLib.h"
#include "GameDevice.h"

#include "../Input/InputMgr.h"

class Game{
protected:
	// コンストラクタ
	Game(const char* windowName = "game", int width = 800, int height = 600, float rate = 1.0f, int window = TRUE, float fps = 60.0f):
		width_(width), height_(height), rate_(rate), window_(window), fps_(fps) {
	}

public:
	// 実行
	int run() {
		GameDevice device(width_, height_, rate_, window_, fps_);


		LPCSTR font_path = "./resources/file/nikumaru.ttf"; // 読み込むフォントファイルのパス
		if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
		ChangeFont("07にくまるフォント");
		start();
		// キーが押されるまでループします
		while (isRunning()){
			update();
			draw();

			device.flip();
		}
		end();
		if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}

		DxLib_End();	//DXLib使用終了処理
		return 0;	//ソフト終了
	}

private:
	// 開始
	virtual void start() {}
	// 更新
	virtual void update() {}
	// 描画
	virtual void draw() {}
	// 終了
	virtual void end() {}
	// 実行中か
	virtual bool isRunning() { return (ProcessMessage() == 0 && ClearDrawScreen() == 0 && InputMgr::GetInstance().IsKeyUp(KeyCode::ESCAPE) == 0); }

private:
	// コピー禁止
	Game(const Game& other) = delete;
	Game& operator = (const Game& other) = delete;

private:
	// ウィンドウの幅
	int width_;
	// ウィンドウの高さ
	int height_;
	// ウインドウの描画比率
	float rate_;
	// ウインドウモードか？
	int window_;
	// FPS
	float fps_;
};
