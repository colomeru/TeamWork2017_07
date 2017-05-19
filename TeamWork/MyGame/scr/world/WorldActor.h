#pragma once
#include "../game/ID.h"
#include "../actor/ActorManager.h"
#include "../actor/ActorPtr.h"
#include <map>
#include <vector>
#include <functional>


struct CollideSelect
{
	ACTOR_ID	otherID;
	COL_ID		colID;
};

class WorldActor
{
public:
	// コンストラクタ
	WorldActor();
	// デストラクタ
	~WorldActor();
	// 更新
	bool Update();
	bool ChangeLaneUpdate();
	bool StartModeUpdate();
	// 描画
	void Draw(const int laneCount=0,const int playerLane=0) const;
	// 追加
	void Add(ACTOR_ID id, ActorPtr actor);
	// クリア
	void Clear();
	// 衝突判定
	void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);

	// 指定グループの巡回
	void EachActor(ACTOR_ID id, std::function<void(Actor&)> func);
	// 指定グループの巡回
	void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const;
	// 指定グループのアクター数を取得
	int Count(ACTOR_ID id) const;

	// メッセージの処理
	void HandleMessage(EventMessage message, void* param);
	// イベントリスナーの追加
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

private:
	// マネージャーを巡回
	void EachActor(std::function<void(ActorManager&)> func);
	// マネージャーを巡回（const版）
	void EachActor(std::function<void(const ActorManager&)> func) const;



private:
	// マネージャーグループ
	std::map<ACTOR_ID, ActorManager>			   managers_;
	// コリジョングループ
	std::map<ActorPtr, std::vector<CollideSelect>> colselect_;

	// イベントリスナー
	typedef std::function<void(EventMessage, void*)> Listener;
	std::vector<Listener> lis_;
};

