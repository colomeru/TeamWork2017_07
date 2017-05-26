#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"
#include "../../player/Player_Head.h"
#include <array>

class Enemys : public Actor
{
public:
	//コンストラクタ
	Enemys(IWorld* world, int laneNum, const Vector2& pos);
	//仮想デストラクタ
	virtual ~Enemys() = default;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() const = 0;

	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;



public:
	//コピー禁止
	Enemys(const Enemys& other) = delete;
	Enemys& operator = (const Enemys& other) = delete;

protected:
	//衝突しているか
	bool isHit_;

};