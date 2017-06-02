#pragma once
#include <list>
#include <map>
#include <functional>
#include <memory>
#include "TweenObject.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Easing.h"

enum EaseType
{
	Linear,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce
};

// イージングマネージャークラス
class TweenManager
{
private:
	// コンストラクタ
	TweenManager();
	// デストラクタ
	~TweenManager();

	/* コピー禁止 */
	TweenManager(const TweenManager&) = delete;
	TweenManager& operator =(const TweenManager&) = delete;

public:
	static TweenManager &GetInstance() {
		static TweenManager t;
		return t;
	}

	// 初期化
	void Initialize();
	// 更新
	void Update(const float deltaTime);
	// 追加
	void Add(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加
	void Add(Vector2* value, const EaseType& type, const Vector2& b, const Vector2& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加
	void Add(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加
	void Add(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加
	void Add(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 追加
	void Add(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ディレイ呼び出し
	void Delay(const float delay, const std::function<void()>& func, float* debug = nullptr);
	// ループ処理（callback呼び出しは1ループ毎 / 必ずデストラクタなりで Cancel を呼ぶ）
	void Loop(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// 特殊ループ処理（callback呼び出しは1ループ毎 / 必ずデストラクタなりで Cancel を呼ぶ）
	void LoopPingPong(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ループ処理（一度のみ）
	void LoopOnce(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, std::function<void()> callback = nullptr, const float s = 1.0f);
	// クリア
	void Clear();
	// 削除
	void Remove();
	// キャンセル（成功した場合trueを返す）
	bool Cancel(const float* value);
	// キャンセル（成功した場合trueを返す）
	bool Cancel(const Vector2* value);
	// キャンセル（成功した場合trueを返す）
	bool Cancel(const Vector3* value);
	// ディレイキャンセル（成功した場合trueを返す）
	bool DelayCancel(const float* value = nullptr);

private:
	// イージング用シェアドポインタ
	using TweenPtr = std::shared_ptr<TweenObject>;
	// イーズリスト
	std::list<TweenPtr> easeList_;
	//
	using TweenFuncParam = std::function<float(float, float, float, float, float)>;
	//
	std::map<EaseType, TweenFuncParam>	easeFuncMap_;
	//
	Easing	ease_;
	// 更新タイプ
	UpdateType	updateType;
};