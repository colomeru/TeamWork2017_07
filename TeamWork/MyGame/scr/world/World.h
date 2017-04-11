#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include <stack>

class World : public IWorld
{
public:
	// コンストラクタ
	World();
	// デストラクタ
	~World();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw() const;
	// クリア
	void Clear();
	// イベントリスナーの追加
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

	// アクターの取得
	WorldActor GetWorldActor() const;

	/* ワールドインターフェース */
	// 追加
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	// 終わっているか？
	virtual bool IsEnd() const;
	// 衝突判定設定
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);
	// メッセージの送信
	virtual void sendMessage(EventMessage message, void* param = nullptr) override;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(Actor&)> func) override;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const override;
	// 指定グループのアクター数を取得
	virtual int Count(ACTOR_ID id) const override;

	// 操作アクターセット
	virtual void PushStackActor(ActorPtr actor);
	// 操作アクターポップ
	virtual void PopStackActor();

private:
	// アクター
	WorldActor	actors_;

	// 受動更新アクター用スタック
	std::stack<ActorPtr>	manualStackActor_;
	// カメラ用スタック
	std::stack<ActorPtr>	stackCamera_;
};