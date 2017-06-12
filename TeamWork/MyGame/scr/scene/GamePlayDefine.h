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
800,//ステージ1
800,//ステージ2
500,//ステージ3
800,//ステージ4
800,//ステージ5
800,//ステージ6
800,//ステージ7
800	//ステージ8
};
//叩きを生成するかどうか
static const bool isSpawnTapper[8]{
false,//1
false,//2
false,//3
true,//4
false,//5
false,//6
false,//7
false//8
};
//鳥を生成するまでの時間
static const int spawnBirfdTime[8]{
	300,//ステージ1
	300,//ステージ2
	300,//ステージ3
	300,//ステージ4
	300,//ステージ5
	300,//ステージ6
	300,//ステージ7
	300	//ステージ8
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
static const int frequencyHairBall = 10;
