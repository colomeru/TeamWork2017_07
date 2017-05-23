#pragma once
#include "../Def.h"
#include "../math/Vector2.h"
#include <stack>
#include <functional>
#include <vector>

// フェードクラス
class FadePanel
{
public:
	enum ColorType
	{
		White,
		Black
	};
	enum FADE_STATUS
	{
		eStandby = 0,	// 実行待ち
		eFadeIn	 = 1,	// フェードイン
		eFadeOut = 2		// フェードアウト
	};


private:
	// コンストラクタ
	FadePanel();
	// デストラクタ
	~FadePanel();

	/* コピー禁止 */
	FadePanel(const FadePanel& other) = delete;
	FadePanel & operator = (const FadePanel& other) = delete;


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
	void  SetInTime(const float sec, const float delay = 0.0f);
	// フェードアウトタイムの取得
	float GetOutTime() const;
	// フェードアウトタイムの設定
	void  SetOutTime(const float sec, const float delay = 0.0f);

	// ディレイタイム取得
	float GetDelayTime() const;

	// コールバック関登録
	void AddCollBack(std::function<void()> collback);

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

	// コールバック関数宣言
	using CallBack = std::function<void()>;
	// コールバックベクター
	std::vector<CallBack>	callbacks_;

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