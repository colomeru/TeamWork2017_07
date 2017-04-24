#include "WorldActor.h"
#include <algorithm>

WorldActor::WorldActor()
{
}

WorldActor::~WorldActor()
{
}

// 更新
void WorldActor::Update()
{
	// 全キャラアップデート
	EachActor([&](ActorManager& manager) { manager.Update(); });

	// 衝突判定
	for (auto& cols : colselect_)
		for (auto& sec : cols.second)
			managers_[sec.otherID].Collide(sec.colID, *cols.first);

	// 当たり判定クリア
	colselect_.clear();

	// 死亡キャラ削除
	EachActor([](ActorManager& manager) { manager.Remove(); });
}

// 描画
void WorldActor::Draw(const int laneCount, const int playerLane) const
{
	// 全キャラ描画
	EachActor([&](const ActorManager& manager) { manager.Draw(laneCount, playerLane); });
}

// 追加
void WorldActor::Add(ACTOR_ID id, ActorPtr actor)
{
	managers_[id].Add(actor);
}

// クリア
void WorldActor::Clear()
{
	// 全キャラクリア
	managers_.clear();
	// 当たり判定クリア
	colselect_.clear();
}

// 衝突判定選択
void WorldActor::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	CollideSelect c = { otherID, colID };
	colselect_[thisActor].push_back(c);
}

// 指定グループの巡回
void WorldActor::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	auto manager = managers_.find(id);
	if (manager == managers_.end()) return;
	manager->second.EachActor(func);
}

// 指定グループの巡回
void WorldActor::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	const auto manager = managers_.find(id);
	if (manager == managers_.cend()) return;
	manager->second.EachActor(func);
}

// 指定グループのアクター数を取得
int WorldActor::Count(ACTOR_ID id) const
{
	auto manager = managers_.find(id);
	if (manager == managers_.end()) return 0;
	return manager->second.Count();
}

// メッセージの処理
void WorldActor::HandleMessage(EventMessage message, void * param)
{
	// ワールドのメッセージ処理
	for (auto& lis : lis_)
		lis(message, param);

	// アクターのメッセージ処理
	for (auto& pair : managers_)
		pair.second.HandleMessage(message, param);
}

// イベントリスナーの追加
void WorldActor::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	lis_.push_back(listener);
}

// マネージャーを巡回
void WorldActor::EachActor(std::function<void(ActorManager&)> func)
{
	std::for_each(managers_.begin(), managers_.end(),
		[&](std::pair<const ACTOR_ID, ActorManager>& e)
	{
		func(e.second);
	});
}

// マネージャーを巡回（const版）
void WorldActor::EachActor(std::function<void(const ActorManager&)> func) const
{
	std::for_each(managers_.cbegin(), managers_.cend(),
		[&](const std::pair<const ACTOR_ID, ActorManager>& e)
	{
		func(e.second);
	});
}
