#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>

class MenuScreen
{
public:
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
	//
	void OpenNextStage();
	
private:
	int stageNum = 0; //ステージ番号
	const float height = WINDOW_HEIGHT / 4.0f * 3.0f; //パネル１のy座標
	Vector2 backPos; //戻るパネルの座標
	Vector2 cursorPos; //カーソルの座標
	int panelNum, cursorNum; //パネル番号、カーソル番号
	std::array<PanelStruct, 9> panel; //PanelStructをステージ数分生成

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir;
	float dis;
	float disN;
};