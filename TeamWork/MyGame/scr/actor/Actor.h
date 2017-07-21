#pragma once
#include "ActorParameter.h"
#include "ActorPtr.h"
#include "../collision/CollisionFunction.h"
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
#include"../Def.h"

#include "DxLib.h"

static const float alphaSetter[2] = { 1.f,0.5f };
//更新及び判定の制限範囲、0=更新範囲の左端,1=更新範囲の右端,2=判定範囲の左端,3=判定範囲の右端
static const int cutSize[4] = { 600,2200 ,400,800};
class Actor
{
public:
	// コンストラクタ
	Actor(IWorld* world, Actor* parent = nullptr);
	// 仮想デストラクタ
	virtual ~Actor();
	// 更新
	virtual void Update() = 0;
	// 描画
	void BaseDraw()const {
		if (!isDraw_)return;
		else
		{
			int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
			drawLane = MathHelper::Abs(drawLane);
			Draw();

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
	virtual void FastUpdate() {
	}
	Vector2 GetDrawAddPos()const {
		return drawAddPos_;
	}
	void FastComUpdate() {
		drawAddPos_ = Vector2::Zero;
		if (world_->isChangeFrame()) { 
			isUpdate_ = true;
		}
		else {
			isUpdate_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
		}
	}
	virtual void LateUpdate() {

	}
	void CommonUpdate() {
		prevPosition_ = position_;
	}
	void LateComUpdate();
	//レーン移動時限定のアップデート、virtualだが、Player以外はoverrideしないようにする事
	virtual bool CamMoveUpdate() {
		CamMoveOnlyUpdate();
		parameter_.spriteAlpha_ = 0.5f;
		if (0 > world_->GetKeepDatas().nextLane_) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}
		drawPos_ = GetDrawPosVect(position_);
		return true;
	}
	virtual void CamMoveOnlyUpdate(){}
	virtual void StartOnlyUpdate() {

	}
	virtual void StartOnlyLateUpdate() {

	}
	bool StartModeUpdate() {
		StartOnlyUpdate();
		isDraw_ = drawPos_.x >= -WINDOW_WIDTH / 2.f&&drawPos_.x <= WINDOW_WIDTH*1.5f;
		isUpdate_ = true;

		StartOnlyLateUpdate();
		return false;
	}
	virtual void CamMoveUp() {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_-laneNum_+2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum+1], laneLerpNum)- defDrawLineChangePosY[targetNum];
	}
	virtual void CamMoveDown() {
		LaneChangeFall();
	}
	Vector2 GetVelocity()const {
		return velocity_;
	}
	Vector2 GetDrawPosVect(const Vector2& pos)const{
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);
		
		int drawLane;
		drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
		
		if (world_->GetIsFreeCamY_()&&drawLane < 0) {

			retPos.y += defDrawLinePosY[drawLane + 1]*1.2f;

			return retPos;
		}

		if (MathHelper::Abs (drawLane) >= 2) {
			retPos.y = -500;
		}
		else {
			retPos.y += defDrawLinePosY[drawLane + 1];

		}
		retPos += drawAddPos_;
		return retPos;
	}
	//lanenumを自由に指定してdrawposを算出
	Vector2 GetFreeActorDrawPos(const Vector2& pos,int laneNum) const{
		Vector2 retPos;

		Vector3 cmpos3d = Vector3(pos.x, pos.y, 0)*world_->GetInv();
		retPos = Vector2(cmpos3d.x, cmpos3d.y);

		int drawLane;
		drawLane = laneNum - world_->GetKeepDatas().playerLane_;

		if (world_->GetIsFreeCamY_() && drawLane < 0) {

			retPos.y += defDrawLinePosY[drawLane + 1] * 1.2f;

			return retPos;
		}

		if (MathHelper::Abs(drawLane) >= 2) {
			retPos.y = -500;
		}
		else {
			retPos.y += defDrawLinePosY[drawLane + 1];

		}
		retPos += drawAddPos_;
		return retPos;
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
	void DrawUpdate() {
		drawPos_ = GetDrawPosVect(position_);
	}
protected:
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param);
	//落ちる時のレーン変更
	virtual void LaneChangeFall() {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
	}
private:
	/* コピー禁止 */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//更新、判定、描画を行うかどうか
	bool isUpdate_;
	bool isCheckCol_;
	//自身が描画する画像のID
	SPRITE_ID spriteId_;
	// ワールド
	IWorld*				world_;
	// コリジョンファンクション
	CollisionFunction	colFunc_;
	// 親
	Actor*				parent_;

	// パラメータ
	ActorParameter	parameter_;
	// 位置
	Vector2			position_;
	//前フレームの位置
	Vector2			prevPosition_;

	//各アクターの描画位置を計算した値
	Vector2			drawPos_;
	//レーン変更時にdrawposに加算する値
	Vector2			drawAddPos_;
	bool isDraw_;
	// 移動量
	Vector2			velocity_;
	//自身が所属しているレーンの番号
	int				laneNum_;
	//自身の角度
	float			angle_;
	
	// ファンクションマップ
	std::map<COL_ID, std::function<CollisionParameter(const Actor&, const Actor&)>> colFuncMap_;
};