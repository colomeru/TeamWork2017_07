#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include "../../stageGenerator/Stage.h"
#include "../../math/Vector3.h"
#include <vector>
#include "../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class MenuScreen
{
public:
	//ステージパネル
	struct PanelStruct
	{
		Vector2 position; //パネルの座標
		bool isDraw; //選択可能か
		float alpha; //パネルのアルファ値
		PanelStruct() :
			position(Vector2::Zero), isDraw(false), alpha(0.0f) {}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//星
	struct StarStruct
	{
		Vector2 position_;
		float isAlpha_;
		float timer_;
	};


	//コンストラクタ
	MenuScreen();
	//デストラクタ
	~MenuScreen();
	void Init();
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
	//星
	void Star();
	//流れ星
	void ShootingStar();
	//カラス
	void Crow();
	//SE
	void SE();


	Stage GetGamePlayStage()const;

//外部クラス用
public:
	bool GetIsBackSelect()const {
		return backSelect;
	}
	bool GetIsTutorialSelect()const {
		return stageNum == 0;
	}
	void InputSelectStage();
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
	int bgHandle; //背景のハンドル
	int builHandle; //ビルのハンドル
	int wwwHandle; //草のハンドル
	Vector2 bgPos_; //背景座標
	Vector2 builPos_; //ビル座標
	Vector2 wwwPos_; //草座標
	float betDis_; //間距離

	//背景色
	std::array<Vector3, 9> bgColor_;
	Vector3 color_;

	//星
	std::array<StarStruct, 50> star_;
	int starNum_;
	float alphaValue_;
	std::array<float, 3> starAlpha_;

	//流れ星
	std::array<StarStruct, 10> sStar_;
	int sStarNum_;
	std::array<float, 10> waitTime_;
	std::array<Vector2, 10> prevPos_;
	std::array<float, 10> ssAlpha_;

	//カラス
	std::array<Vector2, 3> crowPos_;
	std::array<float, 3> interval_;
	std::array<float, 3> cTimer_;
	std::array<bool, 3> spriteTurn_;
	std::array<Vector2, 3> cVelocity_;
	std::array<Vector2, 3> cFrom_;
	std::array<float, 3> cDis_;

	CharacterAnmManager anmManager_;

	std::array<Stage, 9> stageList_;
};