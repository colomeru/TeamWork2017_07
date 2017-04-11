#pragma once

#include "../../Base/Actor.h"
#include "../../../World/IWorld.h"

// プレーヤー連結用Body
class PlayerBodyPoint : public Actor {
public:
	// コンストラクタ
	PlayerBodyPoint(IWorld* world, const Vector2 & position, const int& index);
	// デストラクタ
	~PlayerBodyPoint();
	// 更新処理
	virtual void onUpdate(float deltaTime) override;
	// 描画処理
	virtual void onDraw() const override;
	// 衝突処理
	virtual void onCollide(Actor& other) override;
	// 引力更新処理
	void attract_update(float deltaTime);
	// 引力更新処理
	void clamp_update(int index);
private:
	// 自身のindex番号
	int index_;
	// バネ補間速度
	Vector2 v1_, v2_;
	// テスト用バネ系変数
	float stiffness_;
	float friction_;
	float mass_;

	Vector2 pre_;
};
