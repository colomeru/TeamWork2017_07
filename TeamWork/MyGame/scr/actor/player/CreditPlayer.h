#pragma once
#include "../Actor.h"
#include <vector>
#include "../../math/MathHelper.h"
#include "../../math/MyFuncionList.h"
#include "../../Def.h"
#include "../../graphic/DrawPos.h"
#include "Player.h"

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
	//メッセージ処理
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
	//状態ごとの更新
	void FallUpdate();	//落下中
	void ShootUpdate();	//シュート中
	void BiteUpdate();	//挟み中

private:
	bool operatre_; //操作可能か？
};

