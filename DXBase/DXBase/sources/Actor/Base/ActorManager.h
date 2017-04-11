#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include "Actor.h"
#include "ActorUI.h"

#include "ActorPtr.h"

#include <unordered_map>
#include <list>

enum class ActorGroup;
enum class ShapeType;

class ActorManager {
public:
	// コンストラクタ
	ActorManager();
	// 初期化
	void initialize();
	// 更新
	void update(float deltaTime);
	// 描画
	void draw() const;
	// アクターの追加
	void addActor(ActorGroup group, const ActorPtr& actor);
	//UIアクターの追加
	void addUIActor(const ActorUIPtr & actor);
	// アクターの検索
	ActorPtr findActor(const std::string& name);
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);
	//アクターをグループで取得
	std::forward_list<ActorPtr> GetActors(const ActorGroup group);
	// コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;
private:
	// 衝突判定
	void collide();

	void UiUpdate(float delta);
	void UIDraw()const;
private:
	// ルートノード
	Actor root_;
	// アクターグループ
	std::unordered_map<ActorGroup, ActorPtr> actors_;
	//UIアクター
	std::list<ActorUIPtr> uiActors_;
};

#endif

// end of file
