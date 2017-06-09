#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include"../../stageGenerator/Stage.h"

class MenuScreen
{
public:
	//ステージパネル
	struct PanelStruct
	{
		Vector2 position; //パネルの座標
		bool isDraw; //選択可能か
		float alpha; //パネルのアルファ値
		PanelStruct():
			position(Vector2::Zero),isDraw(false),alpha(0.0f){}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//コンストラクタ
	MenuScreen();
	//デストラクタ
	~MenuScreen();
	//更新
	void Update();
	//描画
	void Draw() const;
	//
	void Action();
	//前のステージをクリアしているか？
	bool CheckPreviousStage(int sNum);
	//次のステージの解放
	void OpenNextStage(int sNum);
	//次のステージが解放されているか？
	bool CheckNextStage(int sNum);
	//パターン１
	void Pattern1Update();
	void Pattern1Draw() const;
	//パターン２
	void Pattern2Update();
	void Pattern2Draw() const;
	//"上"が入力されたか
	bool IsInputUp() const;
	//"下"が入力されたか
	bool IsInputDown() const;
	//"左/A"のいずれかが入力されたか
	bool IsInputLeft() const;
	//"上/下/右"のいずれかが入力されたか
	bool IsInputAny() const;
	
	Stage GetGamePlayStage()const;
private:
	int stageNum = 0; //ステージ番号
	const float height = WINDOW_HEIGHT / 4.0f * 2.0f; //パネル１のy座標
	Vector2 backPos; //戻るパネルの座標
	bool backSelect; //戻るを選択しているか？
	Vector2 cursorPos; //カーソルの座標
	int panelNum, cursorNum; //パネル番号、カーソル番号
	std::array<PanelStruct, 9> panel; //PanelStructをステージ数分生成

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir; //移動方向
	float dis;
	float disN;
	Vector2 from;
	float ease;
	bool test;
	const float MoveTime = 0.5f;

	float timer_;
	Vector2 drawPos;
	Vector2 modify; //総移動距離
	Vector2 moveDis; //移動距離
	Vector2 velocity; //速度
	float mag; //速度倍率

	std::array<Stage,9> stageList_;
};