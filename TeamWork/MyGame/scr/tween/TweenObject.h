#pragma once
#include <functional>

enum UpdateType
{
	Common,		// 通常更新
	Loop,		// ループ更新
	PingPong	// 特殊ループ更新
};

//
using TweenFuncParam = std::function<float(float, float, float, float, float)>;

class TweenObject
{
public:
	// コンストラクタ
	TweenObject(float* value, float b, float c, float d = 1.0f, const std::function<void()>& callback = nullptr, float s = 1.0f);
	// デストラクタ
	~TweenObject();
	// 更新
	void Update(const float deltaTime);
	// 終了しているか？
	bool IsEnd() const;
	// 終了
	void End();
	// 再生
	void Play(float scale = 1.0f);
	// 停止
	void Stop();
	// 変動値ポインタ取得
	float* GetValuePointer() const;

	// 関数登録
	void SetFunction(const TweenFuncParam& func);
	// ループタイプ登録
	void SetLoopType(const UpdateType type);
	// ループ数登録
	void SetLoopCount(int count);

private:
	// 通常更新
	void CommonUpdate(float deltaTime);
	// ループ更新
	void LoopUpdate(float deltaTime);
	// 特殊ループ更新
	void PingPongUpdate(float deltaTime);
	// コールバック呼び出し
	void Invoke();

private:
	// ease関数
	std::function<float(float, float, float, float, float)> func_;
	// 更新関数
	std::function<void(float)> updateFunc_;

	// 変動値
	float*	value_;
	// 開始値
	float	b_;
	// 移動量
	float	c_;
	// 終了時間
	float	d_;
	// Back用
	float	s_;
	// コールバック
	std::function<void()> callback_;
	// 時間
	float	timer_;
	// 再生倍率
	float	timeScale_;
	// 終了フラグ
	bool	isEnd_;

	// ループタイプ
	UpdateType	loopType_;
	// ループ数
	int			loopCount_;
	// 更新時汎用補正値
	float	sine_;
};
