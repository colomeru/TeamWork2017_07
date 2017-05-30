#pragma once
#include <list>
#include <functional>
#include <memory>

enum EaseType
{
	Linear
};


class EaseNode
{
public:
	// コンストラクタ
	EaseNode(float* value, EaseType type, float b, float c, float d = 1.0f, std::function<void()> callback = nullptr);
	// デストラクタ
	~EaseNode();
	// 更新
	void Update(float deltaTime);
	// 終了しているか？
	bool IsEnd() const;

private:
	//
	float* value_;
	//
	std::function<void()> callback_;
	// 終了フラグ
	bool isEnd_;
	//
	float b_;
	//
	float c_;
	//
	float d_;
	//
	float timer_;
};

//
using EasePtr = std::shared_ptr<EaseNode>;

//
class EasingManager
{
private:
	// コンストラクタ
	EasingManager();
	// デストラクタ
	~EasingManager();

	/* コピー禁止 */
	EasingManager(const EasingManager&) = delete;
	EasingManager& operator =(const EasingManager&) = delete;

public:
	static EasingManager &GetInstance() {
		static EasingManager e;
		return e;
	}

	// 初期化
	void Initialize();
	// 更新
	void Update(float deltaTime);
	// 追加
	void Add(EasePtr ease);
	// クリア
	void Clear();
	// 削除
	void Remove();

private:

	//
	std::list<EasePtr> easeList_;

};