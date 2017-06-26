#pragma once
#include "../stageGenerator/Stage.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;


//星3に必要なスコアのリスト
static const int ScoreList[8]{
	1000,//ステージ1
	1500,//ステージ2
	2000,//ステージ3
	2500,//ステージ4
	3000,//ステージ5
	3500,//ステージ6
	4000,//ステージ7
	5000 //ステージ8
};

//風が吹くまでの基本時間(フレーム数)
static const int defWindTime[8]{
1000,//ステージ1
600,//ステージ2
500,//ステージ3
400,//ステージ4
450,//ステージ5
500,//ステージ6
400,//ステージ7
200	//ステージ8
};
//叩きを生成するかどうか
static const bool isSpawnTapper[8]{
false,//1
false,//2
true,//3
true,//4
false,//5
true,//6
true,//7
true//8
};
//鳥を生成するまでの時間
static const int spawnBirfdTime[8]{
	500,//ステージ1
	400,//ステージ2
	400,//ステージ3
	400,//ステージ4
	300,//ステージ5
	200,//ステージ6
	250,//ステージ7
	200	//ステージ8
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
static const int frequencyWind = 25;
//毛玉が飛んでくる確率（％）
static const int frequencyHairBall = 30;
