#ifndef ACTOR_H_
#define ACTOR_H_

#include "DxLib.h"

#include "ActorPtr.h"
#include "HitOpponent.h"
#include "../../Field/FieldPtr.h"
#include "../../Math/Math.h"
#include "../Body/BoundingBox.h"
#include "../Body/BoundingCapsule.h"
#include "../Body/BoundingCircle.h"
#include "../Body/BoundingSegment.h"
#include "../Body/CollisionBase.h"
#include <string>
#include <functional>
#include <forward_list>

class IWorld;

enum class EventMessage;

class Actor {
public:
	// コンストラクタ
	Actor(IWorld* world, const std::string& name, const Vector2& position, const CollisionBase& body = CollisionBase());
	// コンストラクタ
	explicit Actor(const std::string& name = "none");
	// 仮想デストラクタ
	virtual ~Actor() {}
	// 更新
	void update(float deltaTime);
	// 更新
	void late_update(float deltaTime);
	// 描画
	void draw() const;
	// 衝突処理
	void collide(Actor& other);
	// 死亡しているか？
	bool isDead() const;
	// 死亡する
	void dead();
	// 名前を返す
	const std::string& getName() const;
	// 座標を返す
	Vector2 getPosition() const;
	// 座標を取得
	Vector2& position();
	// 回転行列を返す
	Matrix getRotate() const;
	// 変換行列を返す
	Matrix getPose() const;
	// 子の検索
	ActorPtr findCildren(const std::string& name);
	// 子の検索
	ActorPtr findCildren(std::function<bool (const Actor&)> fn);
	// 子同士の衝突
	void collideChildren(Actor& other);
	// 兄弟同士の衝突判定
	void collideSibling();
	// 子の追加
	void addChild(const ActorPtr& child);
	// 子を巡回
	void eachChildren(std::function<void(Actor&)> fn);
	// 子を巡回(const版)
	void eachChildren(std::function<void(const Actor&)> fn) const;
	// 子を削除する
	void removeChildren(std::function <bool(Actor&)> fn);
	// 子を削除する
	void removeChildren();
	// 子を消去
	void clearChildren();
	//子を取得
	std::forward_list<ActorPtr> getChildren();
	// Transformの設定
	void setTransform(Vector2 pos, Matrix rot);
	// Worldの取得
	IWorld* getWorld();
	//Bodyの取得 return:CollisionBase
	CollisionBase getBody();
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);
	// 衝突対象の取得
	HitOpponent getOpponent()const;
	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;
private:
	// メッセージ処理
	virtual void onMessage(EventMessage message, void* param);
	// 更新
	virtual void onUpdate(float deltaTime);

	virtual void onLateUpdate(float deltaTime) ;

	// 描画
	virtual void onDraw() const;
	// 衝突した
	virtual void onCollide(Actor& other);
	// 衝突した
	virtual void hitField(const FieldPtr& field);
public:
	// 衝突判定
	bool isCollide(Actor& other);
	void translate(Vector2& position);
public:
	// ワールド
	IWorld*				world_;
	// 名前
	std::string			name_;
	// 座標
	Vector2				position_;
	// 回転
	Matrix				rotation_;
	// 衝突判定
	CollisionBase		body_;
	// 死亡フラグ
	bool				dead_;

	float				alpha_;

	Matrix inv_;

	HitOpponent opponent_;

private:
	// 子アクター
	std::forward_list<ActorPtr> children_;
};

#endif
