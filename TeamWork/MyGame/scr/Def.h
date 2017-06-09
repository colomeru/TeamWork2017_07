#pragma once

///<summary>
/// ウィンドウ横サイズ
///</summary>
static const int WINDOW_WIDTH = 1920;

///<summary>
/// ウィンドウ縦サイズ
///</summary>
static const int WINDOW_HEIGHT = 1080;

///<summary>
/// 画面色のビット深度
///</summary>
static const int COLOR_BIT_DEPTH = 16;

///<summary>
/// リフレッシュレート
///</summary>
static const int REFRESH_RATE = 60;

///<summary>
/// ゲームタイトル
///</summary>
#define GAMENAME "個人製作"
///<summary>
/// デフォルトのＢＧＭボリューム
///</summary>
static const float BGMVOLUME = 0.8f;

///<summary>
/// デフォルトのＳＥボリューム
///</summary>
static const float SEVOLUME = 0.8f;
#define MOVIE_FILE "気龍_PV.avi"
#define DEMO_MOVIE "気龍_PV"
//デバッグモードなら1,リリースモードなら2
#ifndef NDEBUG
#define BuildMode 1
#else
#define BuildMode 2
#endif