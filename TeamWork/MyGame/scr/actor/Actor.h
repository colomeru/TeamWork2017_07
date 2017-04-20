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
#include"../math/Vector3.h"

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
	void CommonUpdate() {
		prevPosition_ = position_;
	}

	void LateComUpdate() {
		Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
		drawPos_ = Vector2(cmpos3d.x, cmpos3d.y);
	}

	// 自分取得
	Actor* GetActor() const;
	// 親取得
	Actor* GetParent() const;

	Vector2 GetPosition() const{

		return position_;
	}
	Vector2 GetDrawPos()const {
		return drawPos_;
	}
	Vector2 GetPrevPosition() const {
		return prevPosition_;
	}
	float GetAngle() const {
		return angle_;
	}
	int GetLaneNum() const {
		return laneNum_;
	}
	void SetLaneNum(int laneNum){
		laneNum_= laneNum;
	}
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);

protected:
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param);

private:
	CollisionParameter Test_Col(const Actor& other) const;

	// スプライトの当たり判定（２次元のOBB vs OBB）
	CollisionParameter IsHit_OBB_OBB(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Segment(const Actor& sprite2);
	CollisionParameter IsHit_Segment_Segment(const Actor& sprite2);
	CollisionParameter IsHit_Circle_Circle(const Actor& sprite2);
	CollisionParameter IsHit_Circle_Segment(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Circle(const Actor& sprite2);
	CollisionParameter IsHit_OBB_Clothes(const Actor & sprite2);

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
	Vector2			position_;
	Vector2			prevPosition_;

	Vector2			drawPos_;
	// 移動量
	Vector2			velocity_;

	int				laneNum_;

	float			angle_;
private:
	// ファンクションマップ
	std::map<COL_ID, std::function<CollisionParameter(const Actor& sprite2)>> colFunc_;
};