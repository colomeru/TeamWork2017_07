#pragma once

#include "../../../Base/Actor.h"
#include "../../../../World/IWorld.h"
#include "../../../../Animation/Player/PlayerEffect2D.h"
//#include "EventMessage.h"

// プレーヤーエフェクトオブジェ
class PlayerEffectObj : public Actor {
public:
	// コンストラクタ
	PlayerEffectObj(IWorld* world, const Vector2& position, const PlayerEffectID & id, const float& speed = 1.0f, const float& scale = 1.0f);
	// デストラクタ
	~PlayerEffectObj();
	// 更新処理
	virtual void onUpdate(float deltaTime) override;
	// 描画処理
	virtual void onDraw() const override;
private:
	// エフェクトクラス
	PlayerEffect2D effect_;
	// 拡大率
	float scale_;
};