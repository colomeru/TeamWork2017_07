#ifndef BOSS_SCROLL_POINT_H_
#define BOSS_SCROLL_POINT_H_

#include "../../../../../Base/Actor.h"

class BossScrollPoint : public Actor {
public:
	BossScrollPoint(
		IWorld* world,
		const Vector2& position);
	void onUpdate(float deltaTime) override;

public:
	// 中ボスステージ内に入ったかを返します
	bool isInMBossStage();

private:
	// プレイヤーの位置を計算
	void playerPosition(const std::string name);
	// スクロール時の移動
	void scrollMove(float deltaTime);

private:
	bool isPlayerIn_;				// プレイヤーが範囲内に入っているか
	Vector2 prevPlayerDirection_;	// プレイヤーの方向(侵入時)
	ActorPtr player_;				// 侵入したプレイヤー
};

#endif