#pragma once
#include "../Def.h"
#include "../math/Vector2.h"
#include <stack>

class FadePanel
{
public:
	enum ColorType
	{
		White,
		Black
	};

public:
	enum class FADE_STATUS
	{
		STANDBY = 0,	// 実行待ち
		FadeIn	= 1,	// フェードイン
		FadeOut = 2		// フェードアウト
	};


private:
	// コンストラクタ
	FadePanel();
	// デストラクタ
	~FadePanel();

public:
	static FadePanel &GetInstance(){
		static FadePanel f;
		return f;
	}

	// 初期化
	void Initialize();
	// 更新
	void Update(float deltaTime);
	// 描画
	void Draw() const;
	// フェードイン
	void FadeIn();
	// フェードアウト
	void FadeOut(ColorType type = ColorType::Black, float maxAlpha = 1.0f);
	// 変異中か？
	bool IsAction() const;
	// 画面が埋まっているか？
	bool IsFillScreen() const;
	// 画面が透明か？
	bool IsClearScreen() const;

	// フェードインタイムの取得
	float GetInTime() const;
	// フェードインタイムの設定
	void  SetInTime(float sec);
	// フェードアウトタイムの取得
	float GetOutTime() const;
	// フェードアウトタイムの設定
	void  SetOutTime(float sec);

	// ディレイタイム取得
	float GetDelayTime() const;
	// ディレイタイム設定
	void  SetDelayTime(float sec);

private:
	// フェードインアップデート
	void FadeInUpdate(float deltaTime);
	// フェードアウトアップデート
	void FadeOutUpdate(float deltaTime);

private:
	// リソースサイズ
	const Vector2 RES_SIZE = Vector2(800, 600);
	// スクリーンサイズ
	const Vector2 SCALE	   = Vector2((float)WINDOW_WIDTH / RES_SIZE.x, (float)WINDOW_HEIGHT / RES_SIZE.y);

	// ステートスタック
	std::stack<FADE_STATUS>	stateStack_;

	// タイプ
	ColorType	color_;
	// アルファ値
	float		alpha_;
	// アルファ最大値
	float		maxAlpha_;
	// 変異時間
	float		actionTime_;
	// フェードインタイム
	float		inTime_;
	// フェードアウトタイム
	float		outTime_;
	// ディレイタイム
	float		delayTime_;
};