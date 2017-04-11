#pragma once
#include "ActorParameter.h"
#include "ActorPtr.h"
#include "../collision/CollisionParameter.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include "../math/Quaternion.h"
#include "../world/IWorld.h"
#include <functional>
#include <map>

#include "DxLib.h"

class Actor 
{
public:
	// コンストラクタ
	Actor(IWorld* world, Actor* parent = nullptr);
	// 仮想デストラクタ
	virtual ~Actor() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() const = 0;
	// 死亡しているか？
	virtual bool isDead() const { return parameter_.isDead; }
	// 死亡する
	void Dead() { parameter_.isDead = true; };

	// 衝突判定
	void Collide(COL_ID id, Actor& other);
	// パラメータの取得
	ActorParameter GetParameter() const;
	// ポーズの取得
	Matrix GetPose() const;
	// ポーズの設定
	virtual void SetPose(const Matrix& mat);
	// 受動更新
	virtual void OnUpdate();

	// 自分取得
	Actor* GetActor() const;
	// 親取得
	Actor* GetParent() const;

	// メッセージ処理
	void handleMessage(EventMessage message, void* param);

protected:
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param);

private:
	/* コピー禁止 */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ワールド
	IWorld*			world_;
	// 親
	Actor*			parent_;

	// パラメータ
	ActorParameter	parameter_;
	// 位置
	Vector3			position_;
	// 移動量
	Vector3			velocity_;

private:
	// ファンクションマップ
	std::map<COL_ID, std::function<CollisionParameter(const Actor&)>> colFunc_;
};