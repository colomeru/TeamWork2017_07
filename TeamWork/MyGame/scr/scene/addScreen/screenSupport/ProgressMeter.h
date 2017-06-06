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
	ProgressMeter(World* world, int stageLength, int pinPosition);
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
	////上段
	//void UpperLane();
	//void UpperDraw() const;
	////中段
	//void MiddleLane();
	//void MiddleDraw() const;
	////下段
	//void LowerLane();
	//void LowerDraw();	


private:
	World*		world_;

	int stageLen_; //ステージの長さ
	std::vector<PinStruct> pin_; //ピン情報
	int meterNum_; //メーターの本数
	int meterLen_; //メーターの長さ
	int nowLane_; //プレイヤーのいるレーン番号
	Vector2 meterPos_; //メーター座標
	Vector2 pIconPos_; //プレイヤーアイコンの座標
	float pPosY_; //プレイヤーアイコンのy座標
	int prevLane_; //1フレーム前のレーン番号

	//画像のハンドル
	int pinHandle_; //ピン
	int playerHandle_; //カーソル
	int upperHandle_; //レーン１（上段）
	int middleHandle_; //レーン２（中段）
	int lowerHandle_; //レーン３（下段）
	int startHandle_; //スタート
	int goalhandle_; //ゴール

};