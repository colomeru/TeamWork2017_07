#include "World.h"
#include "../actor/Actor.h"

// コンストラクタ
World::World()
{
}

// デストラクタ
World::~World()
{
	Clear();
}

// 初期化
void World::Initialize()
{
	Clear();
}

// 更新
void World::Update()
{
	actors_.Update();
	
	// 受動更新
	if (!manualStackActor_.empty())
		manualStackActor_.top()->OnUpdate();

	// カメラ更新
	//if (!stackCamera_.empty())
	//	stackCamera_.top()->OnUpdate();
}

// 描画
void World::Draw() const
{
	actors_.Draw();
}

// クリア
void World::Clear()
{
	// スタック消去
	while (!manualStackActor_.empty())
		manualStackActor_.pop();
	while (!stackCamera_.empty())
		stackCamera_.pop();

	// アクター消去
	actors_.Clear();
}

// イベントリスナーの追加
void World::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	actors_.AddEventMessageListener(listener);
}

// アクターの取得
WorldActor World::GetWorldActor() const
{
	return actors_;
}

// 追加
void World::Add(ACTOR_ID id, ActorPtr actor)
{
	actors_.Add(id, actor);
}

// 
bool World::IsEnd() const
{
	return false;
}

// 衝突判定
void World::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	actors_.SetCollideSelect(thisActor, otherID, colID);
}

// メッセージの送信
void World::sendMessage(EventMessage message, void * param)
{
	actors_.HandleMessage(message, param);
}

// 指定グループの巡回
void World::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	actors_.EachActor(id, func);
}

// 指定グループの巡回
void World::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	actors_.EachActor(id, func);
}

int World::Count(ACTOR_ID id) const
{
	return actors_.Count(id);
}

// 操作アクターセット
void World::PushStackActor(ActorPtr actor)
{
	manualStackActor_.push(actor);
}

// 操作アクターポップ
void World::PopStackActor()
{
	if (!manualStackActor_.empty())
		manualStackActor_.pop();
}