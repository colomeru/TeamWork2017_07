#include "ActorManager.h"
#include "Actor.h"
#include <algorithm>

// 更新
void ActorManager::Update()
{
	for (auto& actor : actorPtr)
	{
		actor->CommonUpdate();
		actor->Update();
		actor->LateComUpdate();
	}
}

// 描画
void ActorManager::Draw() const
{
	for (auto& actor : actorPtr)
		actor->Draw();
}

// 追加
void ActorManager::Add(ActorPtr actor_)
{
	actorPtr.push_back(actor_);
}

// クリア
void ActorManager::Clear()
{
	actorPtr.clear();
}

// 削除
void ActorManager::Remove()
{
	actorPtr.remove_if([](ActorPtr actor)
	{
		return actor->isDead();
	});
}

// 衝突判定
void ActorManager::Collide(COL_ID id, Actor & other)
{
	for (auto& actor : actorPtr)
		other.Collide(id, *actor);
}

// 衝突判定
void ActorManager::Collide(COL_ID id, ActorManager & other)
{
	for (auto& actor : actorPtr)
		other.Collide(id, *actor);
}

// 子を巡回
void ActorManager::EachActor(std::function<void(Actor&)> func)
{
	for (auto& actor : actorPtr)
		func(*actor);
}

// 子を巡回（const版）
void ActorManager::EachActor(std::function<void(const Actor&)> func) const
{
	for (auto& actor: actorPtr)
		func(*actor);
}

// アクター数を返す
int ActorManager::Count() const
{
	return (int)actorPtr.size();
}

// メッセージ処理
void ActorManager::HandleMessage(EventMessage message, void * param)
{
	EachActor([&](Actor& other)
	{
		other.handleMessage(message, param); 
	});
}

std::list<ActorPtr> ActorManager::GetList() const
{
	return actorPtr;
}
