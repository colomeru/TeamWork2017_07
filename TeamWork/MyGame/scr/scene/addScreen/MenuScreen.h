#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include "../../stageGenerator/Stage.h"
#include "../../math/Vector3.h"
#include "../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"
#include "screenSupport/MenuCrow.h"

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
		Vector2 position_;	//座標
		float isAlpha_;		//アルファ値
		float timer_;		//タイマ
		float scale_;		//拡大値
		float waitTime_;	//待機時間
		Vector2 prevPos_;	//戻り座標
	};

	//コンストラクタ
	MenuScreen();
	//デストラクタ
	~MenuScreen();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw() const;
	//次のステージの解放
	void OpenNextStage(int sNum);
	//次のステージが解放されているか？
	bool CheckNextStage(int sNum);
	//"上"が入力されたか
	bool IsInputUp() const;
	//"下"が入力されたか
	bool IsInputDown() const;
	//"左/A"のいずれかが入力されたか
	bool IsInputLeft() const;
	//"上/下/右"のいずれかが入力されたか
	bool IsInputAny() const;
	//チェックボタンが押されたか
	bool IsInputCheck() const;
	//星
	void Star();
	//流れ星
	void ShootingStar();
	//カラス
	void Crow();
	//SE
	void SE();
	//終了
	void End();
	//ステージを取得
	Stage GetGamePlayStage()const;

	//外部クラス用
public:
	//「戻る」が選択されているか？
	bool GetIsBackSelect()const {
		return backSelect_;
	}
	//チュートリアルステージが選択されているか？
	bool GetIsTutorialSelect()const {
		return stageNum_ == 0;
	}
	//ステージをセット
	void InputSelectStage();
	//背景リセット
	void ResetBG();
	//ステージ番号を取得
	int GetStageNumber();

private:
	const float Height = WINDOW_HEIGHT / 4.0f * 2.0f;	//パネル１のy座標
	const float MoveTime = 0.5f;						//移動にかかる時間
	const float BetDis = 339.0f;						//間距離
	const int StarNum = 10;								//星の個数
	const Vector2 BgPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 475.0f);		//背景座標
	const Vector2 BuilPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 285.0f);	//ビル座標
	const Vector2 WwwPos = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT);						//草座標
	const float OffSet = 300.0f;

	int stageNum_;						//ステージ番号
	bool backSelect_;					//戻るを選択しているか？
	Vector2 cursorPos_;					//カーソルの座標
	std::array<PanelStruct, 9> panel;	//PanelStructをステージ数分生成

	//
	float dis_;		//目的値
	Vector2 from_;	//移動座標

	Vector3 color_;	//背景色

	//星
	float alphaValue_;					//アルファ変更値
	std::array<float, 3> starAlpha_;	//アルファ値

	//流れ星
	std::array<StarStruct, 10> sStar_;	//流れ星構造体

	//カラス
	std::array<Vector2, 3> crowPos_;	//カラス座標
	std::array<float, 3> interval_;		//繰り返し時間
	std::array<float, 3> cTimer_;		//カラスタイマ
	std::array<Vector2, 3> cVelocity_;	//カラス移動量
	std::array<Vector2, 3> cFrom_;		//目標値
	std::array<float, 3> cDis_;			//移動座標
	std::array<MenuCrow, 3> crow_;		//カラス

	CharacterAnmManager anmManager_;	//アニメーション

	std::array<Stage, 9> stageList_;	//ステージリスト
};