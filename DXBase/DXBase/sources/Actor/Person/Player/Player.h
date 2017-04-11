#pragma once

#include "../../Base/Actor.h"
#include "../../../World/IWorld.h"

#include "PlayerPtr.h"

// プレーヤー
class Player : public Actor {
public:
	// コンストラクタ
	Player(IWorld* world, const Vector2& position);
	// デストラクタ
	~Player();
	// 更新処理
	virtual void onUpdate(float deltaTime) override;
	void setClampPoint(Vector2 point);
private:
	// body中心座標
	Vector2 center();
	// body生成
	void create_bodys();
	// 指定stateの更新処理
	void update_state(float deltaTime);
	// 接続処理
	void connect();
	// 接続可能かどうか
	bool is_connectable();
	// 死亡したかどうか
	bool is_dead();
private:
	// body(青)
	PlayerBodyPtr butty_;
	// body(赤)
	PlayerBodyPtr retty_;
	Vector2 prePos_;
};

