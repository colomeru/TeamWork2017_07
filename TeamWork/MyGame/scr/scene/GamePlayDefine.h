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

//画面内の服が揺れる確率（％）
static const int frequencyWind[8]{
	25,//ステージ1
	25,//ステージ2
	25,//ステージ3
	25,//ステージ4
	25,//ステージ5
	25,//ステージ6
	25,//ステージ7
	25 //ステージ8
};

//毛玉が飛んでくる確率（％）
static const int frequencyHairBall[8]{
	12,//ステージ1
	12,//ステージ2
	12,//ステージ3
	12,//ステージ4
	12,//ステージ5
	12,//ステージ6
	12,//ステージ7
	12 //ステージ8
};

//毛玉が飛んでくる頻度（フレーム数）
static const int hairballCnt[8]{
	60,//ステージ1
	60,//ステージ2
	60,//ステージ3
	60,//ステージ4
	60,//ステージ5
	60,//ステージ6
	60,//ステージ7
	60 //ステージ8
};

//ゲーム開始時の全体時間
static const float startEase = 4.f;
//ボタン押してるときに進める時間
static const float startAddTime = 0.005f;
