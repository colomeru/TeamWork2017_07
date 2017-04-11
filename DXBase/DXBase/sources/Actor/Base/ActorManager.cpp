#include "ActorManager.h"
#include "ActorGroup.h"

#include "../Body/ShapeType.h"

#include <algorithm>
// コンストラクタ
ActorManager::ActorManager() {
	initialize();
}

// 初期化
void ActorManager::initialize() {
	actors_[ActorGroup::Effect] = std::make_shared<Actor>();
	actors_[ActorGroup::Player] = std::make_shared<Actor>("PlayerManager");
	actors_[ActorGroup::Player_Collider] = std::make_shared<Actor>();
	actors_[ActorGroup::Player_AttackRange] = std::make_shared<Actor>();
	actors_[ActorGroup::Enemy] = std::make_shared<Actor>("EnemyManager");
	actors_[ActorGroup::Enemy_AttackRange] = std::make_shared<Actor>();
	actors_[ActorGroup::EnemyBullet] = std::make_shared<Actor>();
	actors_[ActorGroup::Enemy_Spawner] = std::make_shared<Actor>();
	actors_[ActorGroup::EffectBack] = std::make_shared<Actor>();
	actors_[ActorGroup::BACKGRAUND] = std::make_shared<Actor>();
	actors_[ActorGroup::Field] = std::make_shared<Actor>();
	actors_[ActorGroup::SCROOLSTOP] = std::make_shared<Actor>("ScroolStop");
	actors_[ActorGroup::PuyoVertex] = std::make_shared<Actor>();
	actors_[ActorGroup::Tubo] = std::make_shared<Actor>();
	actors_[ActorGroup::Item] = std::make_shared<Actor>();
	root_.clearChildren();
	root_.addChild(actors_[ActorGroup::Effect]);
	root_.addChild(actors_[ActorGroup::EnemyBullet]);
	root_.addChild(actors_[ActorGroup::Player_Collider]);
	root_.addChild(actors_[ActorGroup::Player_AttackRange]);
	root_.addChild(actors_[ActorGroup::Enemy]);
	root_.addChild(actors_[ActorGroup::Enemy_AttackRange]);
	root_.addChild(actors_[ActorGroup::Enemy_Spawner]);
	root_.addChild(actors_[ActorGroup::Player]);
	root_.addChild(actors_[ActorGroup::BACKGRAUND]);
	root_.addChild(actors_[ActorGroup::Field]);
	root_.addChild(actors_[ActorGroup::SCROOLSTOP]);
	root_.addChild(actors_[ActorGroup::PuyoVertex]);
	root_.addChild(actors_[ActorGroup::EffectBack]);
	root_.addChild(actors_[ActorGroup::Tubo]);
	root_.addChild(actors_[ActorGroup::Item]);
}

// 更新
void ActorManager::update(float deltaTime) {
	root_.update(deltaTime);
	collide();
	root_.late_update(deltaTime);
	root_.removeChildren();
	UiUpdate(deltaTime);
}

// 描画
void ActorManager::draw() const {
	root_.draw();
	UIDraw();
}

// アクターの追加
void ActorManager::addActor(ActorGroup group, const ActorPtr& actor) {
	actors_[group]->addChild(actor);
}
void ActorManager::addUIActor(const ActorUIPtr & actor) {
	uiActors_.push_back(actor);
}

// アクターの検索
ActorPtr ActorManager::findActor(const std::string& name) {
	return root_.findCildren(name);
}

// メッセージ処理
void ActorManager::handleMessage(EventMessage message, void* param) {
	root_.handleMessage(message, param);
}

std::forward_list<ActorPtr> ActorManager::GetActors(const ActorGroup group) {
	return actors_[group]->getChildren();
}

// 衝突判定
void ActorManager::collide() {
	actors_[ActorGroup::Player]->collideChildren(*actors_[ActorGroup::Player_Collider]);
	actors_[ActorGroup::Player]->collideChildren(*actors_[ActorGroup::Enemy]);
	actors_[ActorGroup::Player_Collider]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::Player]->collideChildren(*actors_[ActorGroup::Enemy_AttackRange]);
	actors_[ActorGroup::Player]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::Enemy]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::Enemy]->collideChildren(*actors_[ActorGroup::Player_Collider]);
	actors_[ActorGroup::EnemyBullet]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::PuyoVertex]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::Player]->collideChildren(*actors_[ActorGroup::Tubo]);
	actors_[ActorGroup::Tubo]->collideChildren(*actors_[ActorGroup::Field]);
	actors_[ActorGroup::Item]->collideChildren(*actors_[ActorGroup::Player_Collider]);
}

void ActorManager::UiUpdate(float delta) {
	for (auto& i : uiActors_) {
		i->update(delta);
	}
	uiActors_.erase(std::remove_if(uiActors_.begin(), uiActors_.end(), [](ActorUIPtr ui)
	{return ui->isDead(); }), uiActors_.end());
}

void ActorManager::UIDraw() const {
	for (auto& i : uiActors_) {
		i->draw();
	}
}
