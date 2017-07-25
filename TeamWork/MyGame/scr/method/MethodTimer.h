#pragma once
#include<functional>
#include<vector>

class MethodTimer {
public:
	//登録した関数を順番に呼び出すクラス
	MethodTimer();
	~MethodTimer();

	//登録されている関数を全て削除し、カウントを0にする
	void Initialize();

	//Action関数内でInitializeする場合は此方
	void InnerInitialize();
	//カウントを0にする
	void NumReset();
	//空の関数を追加する
	void AddEmpty(int size);
	//関数を追加登録する
	void Add(std::function<void()> func);
	//登録された関数を実行する、カウントが最大値を超えたら、何もせずにreturnする
	void Action();
	//登録された最大値のカウントに移動し、関数を実行する
	void ToLastAction();
private:
	//空の関数処理カウントを進める
	void CountEmpty(int count);

private:
	//呼び出し回数カウンタ
	int count_;

	//空の呼び出し回数リスト
	std::vector<int> emptyCount_;
	std::vector<int> MAX_EMPTY_COUNT;

	//関数リスト
	std::vector<std::function<void()>> targetFuncList_;
};