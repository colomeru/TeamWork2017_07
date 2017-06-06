#pragma once
#include "../../../Def.h"
#include "../../../math/Vector2.h"
#include <array>
#include "../../../world/World.h"

class World;
class ProgressMeter
{
private:
	struct PinStruct
	{
		int lane;
		int posX;
	};
public:
	//コンストラクタ
	ProgressMeter(){}
	//コンストラクタ
	ProgressMeter(World* world, int stageLength);
	//デストラクタ
	~ProgressMeter();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw() const;
	//終了
	void End();


private:
	World*		world_;

	int stageLen_; //ステージの長さ
	int meterNum_; //メーターの本数
	int meterLen_; //メーターの長さ
	int nowLane_; //プレイヤーのいるレーン番号
	Vector2 meterPos_; //メーター座標
	Vector2 pIconPos_; //プレイヤーアイコンの座標
	float pPosY_; //プレイヤーアイコンのy座標
	int prevLane_; //1フレーム前のレーン番号
	int dis_; //メーター間距離

	//画像のハンドル
	int pinHandle_; //ピン
	int pIconHandle_; //プレイヤーアイコン
	int laneHandle_; //レーン
	int startHandle_; //スタート
	int goalHandle_; //ゴール

	//画像のサイズ
	Vector2 pinSize_; //ピン
	Vector2 pIconSize_; //プレイヤーアイコン
	Vector2 laneSize_; //レーン１
	Vector2 startSize_; //スタート
	Vector2 goalSize_; //ゴール

};