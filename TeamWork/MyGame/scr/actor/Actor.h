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
#include"../world/World.h"
#include"../graphic/Sprite.h"

#include "DxLib.h"

static const float alphaSetter[2] = { 1.f,0.5f };
static const int cutSize[2] = { 500,1800 };
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
	void BaseDraw()const {
		if (!isDraw_ || !isUpdate_)return;
		else
		{
			int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
			drawLane = MathHelper::Abs(drawLane);
			//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaSetter[drawLane]);
			Draw();
			//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		}
	}
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
	void UpdateList() {
		if (!isUpdate_)return;
		Update();
	}
	void FastUpdate() {
		isUpdate_ = (world_->GetKeepDatas().playerPos_.x - position_.x < cutSize[0] && position_.x - world_->GetKeepDatas().playerPos_.x < cutSize[1]);
	}
	virtual void LateUpdate() {

	}
	void CommonUpdate() {
		prevPosition_ = position_;
	}
	void LateComUpdate() {
		Vector3 cmpos3d = Vector3(position_.x, position_.y, 0)*world_->GetInv();
		drawPos_ = Vector2(cmpos3d.x, cmpos3d.y);


		int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		if (drawLane >= 2) {
			isDraw_ = false;
		}
		else {
			isDraw_ = true;
			drawPos_.y += defDrawLinePosY[drawLane + 1];

			drawLane = MathHelper::Abs(drawLane);
			spriteAlpha_ = alphaSetter[drawLane];
		}
	}

	// 自分取得
	Actor* GetActor() const;
	// 親取得
	Actor* GetParent() const;

	Vector2 GetPosition() const {

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
	bool GetIsUpdate()const {
		return isUpdate_;
	}
	bool GetIsDraw()const {
		return isDraw_;
	}
	void SetLaneNum(int laneNum) {
		laneNum_ = laneNum;
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
	CollisionParameter IsHit_Clothes_OBB(const Actor & sprite2);

	/* コピー禁止 */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//更新、判定、描画を行うかどうか
	bool isUpdate_;
	SPRITE_ID spriteId_;
	float spriteAlpha_;
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
	bool isDraw_;
	// 移動量
	Vector2			velocity_;

	int				laneNum_;

	float			angle_;
private:
	// ファンクションマップ
	std::map<COL_ID, std::function<CollisionParameter(const Actor& sprite2)>> colFunc_;
};