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
#include"../Def.h"

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

	void UpdateList();
	virtual void FastUpdate();
	Vector2 GetDrawAddPos()const;
	void FastComUpdate();
	virtual void LateUpdate();
	void CommonUpdate();
	void LateComUpdate();
	//レーン移動時限定のアップデート、virtualだが、Player以外はoverrideしないようにする事
	virtual bool CamMoveUpdate();
	virtual void CamMoveOnlyUpdate();
	virtual void StartOnlyUpdate();
	virtual void StartOnlyLateUpdate();
	bool StartModeUpdate();
	virtual void CamMoveUp();
	virtual void CamMoveDown();
	Vector2 GetVelocity()const;
	Vector2 GetDrawPosVect(const Vector2& pos)const;
	//lanenumを自由に指定してdrawposを算出
	Vector2 GetFreeActorDrawPos(const Vector2& pos, int laneNum) const;

	// 自分取得
	Actor* GetActor() const;
	// 親取得
	Actor* GetParent() const;

	Vector2 GetPosition() const;
	Vector2 GetDrawPos()const;
	Vector2 GetPrevPosition() const;
	float GetAngle() const;
	int GetLaneNum() const;
	bool GetIsUpdate()const;
	bool GetIsDraw()const;
	void SetLaneNum(int laneNum);
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);
	void DrawUpdate();
protected:
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param);
	//落ちる時のレーン変更
	virtual void LaneChangeFall();
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