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
	700,//ステージ2
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

//画面内の服が揺れる確率（％）
static const int frequencyWind[8]{
	10,//ステージ1
	15,//ステージ2
	20,//ステージ3
	30,//ステージ4
	30,//ステージ5
	30,//ステージ6
	30,//ステージ7
	40 //ステージ8
};

//毛玉が飛んでくる確率（％）
static const int frequencyHairBall[8]{
	0,//ステージ1
	10,//ステージ2
	15,//ステージ3
	20,//ステージ4
	20,//ステージ5
	50,//ステージ6
	100,//ステージ7
	100 //ステージ8
};

//毛玉が飛んでくる頻度（フレーム数）
static const int hairballCnt[8]{
	100,//ステージ1
	70,//ステージ2
	60,//ステージ3
	60,//ステージ4
	60,//ステージ5
	70,//ステージ6
	60,//ステージ7
	40 //ステージ8
};

//ゲーム開始時の全体時間
static const float startEase = 4.f;
//ボタン押してるときに進める時間
static const float startAddTime = 0.005f;
