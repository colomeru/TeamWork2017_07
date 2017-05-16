#include "ActorManager.h"
#include "Actor.h"
#include <algorithm>

// 更新
void ActorManager::Update()
{
	for (auto& actor : actorPtr)
	{
		actor->FastComUpdate();
		actor->FastUpdate();
		actor->CommonUpdate();
		actor->UpdateList();
		actor->LateComUpdate();
		actor->LateUpdate();
	}
}

bool ActorManager::CamMoveUpdate()
{
	bool isCLaneEnd_;
	for (auto& actor : actorPtr)
	{
		isCLaneEnd_=actor->CamMoveUpdate();
	}
	return isCLaneEnd_;
}

bool ActorManager::StartModeUpdate()
{
	for (auto& actor : actorPtr)
	{
		actor->LateComUpdate();
		actor->StartModeUpdate();
	}
	return false;
}

// 描画
void ActorManager::Draw(const int laneCount, const int playerLane) const
{
	// レーン数だけ描画
	for (int i = 0; i <= laneCount; i++)
	{
		// プレイヤーとのレーン差が1以上なら省略
		auto dif = playerLane - i;
		if (MathHelper::Abs(dif) > 1)
			continue;

		for (auto& actor : actorPtr)
		{
			auto thisLane = actor->GetLaneNum();
			// 描画レーンとアクターのレーンが合っていたら描画
			if (thisLane == i && (actor->GetIsUpdate() && actor->GetIsDraw()))
				actor->Draw();

			// 描画はActor側で値を決めるようにする
			//actor->BaseDraw();
		}
	}
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
