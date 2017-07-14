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
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	//重力リセット
	void GravityReset();
	//1で左隣の、未入力で右隣のHeadに回転し、長さをリセットする
	void PHeadLengthReset();
	//死亡した頭を全て回復
	void AllResurrectHead();
	//多重振り子の初期化
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);

private:
	void FallUpdate();
	//
	void ShootUpdate();
	//
	void BiteUpdate();

private:
	bool operatre_; //操作可能か？
};

