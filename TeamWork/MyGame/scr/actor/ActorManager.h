#pragma once
#include "ActorPtr.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include <functional>
#include <list>

class ActorManager
{
public:
	// コンストラクタ
	ActorManager() = default;
	// デストラクタ
	~ActorManager() = default;
	// 更新
	void Update();
	bool CamMoveUpdate();
	bool StartModeUpdate();
	// 描画
	void Draw(const int laneCount=0, const int playerLane=0) const;
	// 追加
	void Add(ActorPtr actor_);
	// 削除
	void Clear();
	// 削除
	void Remove();
	// 衝突判定
	void Collide(COL_ID id, Actor& other);
	// 衝突判定
	void Collide(COL_ID id, ActorManager& other);

	// 子を巡回
	void EachActor(std::function<void(Actor&)> func);
	// 子を巡回（const版）
	void EachActor(std::function<void(const Actor&)> func) const;
	// アクター数を返す
	int Count() const;
	// メッセージ処理
	void HandleMessage(EventMessage message, void* param);
	// リストの取得
	std::list<ActorPtr> GetList() const;

	void DrawUpdate();
private:
	std::list<ActorPtr>	actorPtr;
};