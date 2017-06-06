#pragma once
#include "../stageGenerator/Stage.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;
//風が吹くまでの基本時間(フレーム数)
static const int defWindTime[8]{
800,//ステージ1
800,//ステージ2
800,//ステージ3
800,//ステージ4
800,//ステージ5
800,//ステージ6
800,//ステージ7
800	//ステージ8
};

//static const void setWindTime(Stage name) {
//	switch (name)
//	{
//	case Stage::Stage1: {
//		defWindTime_ = 800;
//	}
//	case Stage::Stage2: {
//		defWindTime_ = 1000;
//	}
//	case Stage::Stage3: {
//		defWindTime_ = 1200;
//	}
//	}
//}
//ゲーム開始時の全体時間
static const float startEase = 4.f;
//ボタン押してるときに進める時間
static const float startAddTime = 0.005f;
//画面内の服が揺れる確率（％）
static const int frequencyWind = 30;
//毛玉が飛んでくる確率（％）
static const int frequencyHairBall = 30;
