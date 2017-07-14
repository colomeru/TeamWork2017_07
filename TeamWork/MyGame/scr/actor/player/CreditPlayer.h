#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include "../../scene/Credit2Scene.h"

class Player_Head;
class Player_Sword;

class CreditPlayer : public Player
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//コンストラクタ
	CreditPlayer(IWorld* world, int maxLaneSize = 3, int startLane = 1);
	//デストラクタ
	~CreditPlayer();
	//更新
	virtual void Update() override;
	//最初の更新
	virtual void FastUpdate() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	//
	virtual void CamMoveUp()override;
	//
	virtual void CamMoveDown() override;
	//
	virtual void LaneChangeFall() override;
	//重力リセット
	void GravityReset();
	//1で左隣の、未入力で右隣のHeadに回転し、長さをリセットする
	void PHeadLengthReset();
	//死亡した頭を全て回復
	void AllResurrectHead();
	//
	void SetBiteSprite();
	//多重振り子の初期化
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	//
	void SetPosition(Vector2 position);
	//本体の座標を返す
	Vector2 GetPlayerPosition();

private:
	void FallUpdate();
	//
	void ShootUpdate();
	//
	void BiteUpdate();

private:
	bool operatre_; //操作可能か？
};

