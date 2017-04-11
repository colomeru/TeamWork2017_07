#pragma once

#include "../../Base/Actor.h"
#include "../../../World/IWorld.h"

#include "PlayerPtr.h"
#include "State/States/Union/PlayerStateMgr_Union.h"
#include "../../../Renderer/Puyo_Texture_K.h"

// プレーヤーコネクタ
class PlayerConnector : public Actor {
public:
	// コンストラクタ
	PlayerConnector(IWorld* world, const Vector2 & position, PlayerBodyPtr& butty, PlayerBodyPtr& retty);
	// デストラクタ
	~PlayerConnector();
	//// 更新処理
	//virtual void onUpdate(float deltaTime) override;

	// 継承無視の更新処理
	void onUpdate(float deltaTime);
	// 描画処理
	virtual void onDraw() const override;

	void point_chase(float deltaTime);

	void create_point(int point_num = 0);

	Vector2 get_point(int index);

	//強制移動関数(速度版)
	void ForcedMove(Vector2 velocity);

public:
	PlayerStateMgr_Union stateMgr_;
private:
	// ダメージを受けたかどうか
	bool is_damaged();
	// クリアしたかどうか
	bool is_cleared();

	void puyoUpdate();
private:
	float timer_;

	PlayerBodyPtr butty_;
	PlayerBodyPtr retty_;
	std::vector<PlayerBodyPointPtr> points;

	//ぷよテクスチャ
	PuyoTextureK* mPuyo;
	//ひっぱる力
	float mPower;
	//補間
	Vector2 mPuyoResPos;
	Vector2 mPuyoPos;
	Vector2 mPuyoVelo;
	//ぷよする時間
	float mPuyoTimer;
	bool mPuyoFlag;
};
