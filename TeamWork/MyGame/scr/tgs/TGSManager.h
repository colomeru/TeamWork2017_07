#pragma once


//TGS用のゲーム強制終了クラス
class TGSManager {
private:
	TGSManager();
public:
	static TGSManager& GetInstance();

	//経過時間、タイマーの有効無効を初期化
	void Initialize();
	//時間更新から、終了判定、処理までを行う
	bool Update();

	//ゲームを終了させる
	void GameEnd();
//GetSet関数
public:
	void SetActive(bool isActive);

//判定関数
private:
	bool CheckExit_Input()const;
	bool CheckExit_Timer()const;

//更新処理関数
	//時間を更新する
	void AddTime();
	//タイマーをリセットするか判断
	void CheckIsReset();
	//
	void ResetTime();
	//強制終了タイマー
	float timeCount_;
	//タイマーの有効無効
	bool isActive_;

private:
	const float EXIT_TIME_{ 120.0f };
};