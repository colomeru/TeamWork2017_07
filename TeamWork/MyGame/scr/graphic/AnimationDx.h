#pragma once

class AnimationDx
{
public:
	// コンストラクタ
	explicit AnimationDx() = default;
	// デストラクタ
	~AnimationDx() = default;
	// 更新
	void Update(const float frameTime);
	// 描画
	void Draw() const;
	// アニメーション切り替え
	void ChangeAnim(const int motion, const float frame = 0.0f);
	// モデルハンドル登録
	void SetHandle(const int& handle);
	// アニメーションエンドフラグ
	bool IsAnimEnd() const;
	// 今のアニメーションフレームを受け取る
	float GetCurrentAnimFrame() const;
	// モーションの最終フレームを受け取る
	float GetEndAnimFrame(const int motion) const;

private:
	// コピーコンストラクタ
	AnimationDx(const AnimationDx& other) = delete;
	// 代入演算子
	AnimationDx& operator =(const AnimationDx& other) = delete;

private:
	// ブレンドレートタイム
	const float RATE_TIME = 0.1f;
	// モデルハンドル
	int		modelHandle_;
	// 今モーション
	int		motion_;
	// 前モーション
	int		prevMotion_;
	// アニメーションタイマー
	float	animTimer_;
	// 前アニメーションタイマー
	float	prevAnimTimer_;
	// アニメーションマックスタイム
	float	maxAnimTime_;
	// アニメーション終了フラグ
	bool	isAnimEnd_;
	// ブレンドレート
	float	rate_;
	// 今アニメーション
	int		anim_;
	// 前アニメーション
	int		prevAnim_;
};
