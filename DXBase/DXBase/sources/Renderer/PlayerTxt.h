#pragma once
#include "TxtRead.h"
#include "../Math/Vector2.h"
enum PlayerEnum
{
	BUTTY,
	RETTY,
	COCOCO
};
class PlayerTxt {
public:
	struct TxtState
	{
		std::string txt;
		PlayerEnum speekPlayer;
		bool lineFlag;
	};
	PlayerTxt(char * fileName, int lineNum);
	~PlayerTxt();
	void Update();
	void Draw(const Vector2& pos,float size);

	//イベントが終わったかどうか
	bool isEndEvent();
	//現在喋っているプレイヤー取得
	PlayerEnum GetSpeekPlayer();
private:
	//ステージでしゃべる全部
	std::vector<TxtState> txts_;
	//進んだ会話カウント
	int speekCount_;
	//終わる会話カウント
	int speedEndCount_;
	//タイマー
	float speekTimer_;
	//文字がどんどん出てくるカウント
	int characterCount_;
	int characterCountLine_;
	//表示用テキスト
	std::string drawTxt_;
	//改行用表示テキスト
	std::string drawLineTxt_;
	//前に喋っていたやつ
	PlayerEnum prePlayer_;
	//現在喋っている奴
	PlayerEnum nowPlayer_;
	//喋るかどうか
	bool isSpeekFlag_;
	//喋り終わったら
	bool isSpeekEnd_;
	//次喋る間の時間
	float nextSpeekTimer_;
	//全ての喋りが終わったかどうか
	bool isAllSpeekEnd_;
};