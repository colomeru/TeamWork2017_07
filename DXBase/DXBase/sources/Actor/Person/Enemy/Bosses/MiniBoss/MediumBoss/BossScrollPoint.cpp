#include "BossScrollPoint.h"
#include "../../../../Player/PlayerBody.h"
#include "../../../../Player/PlayerConnector.h"
#include "../../../../../../World/IWorld.h"
#include "../../../../../../Define.h"

BossScrollPoint::BossScrollPoint(IWorld * world, const Vector2 & position) : 
	Actor(world, "BossScrollPoint", 
		position + Vector2::Right * (CHIPSIZE / 3.0f), 
		CollisionBase()),
	isPlayerIn_(false),
	prevPlayerDirection_(Vector2::Zero),
	player_(nullptr)
{
}

void BossScrollPoint::onUpdate(float deltaTime)
{
	// プレイヤーが範囲内にいるか
	playerPosition("PlayerBody1");
	playerPosition("PlayerBody2");
	if (!isPlayerIn_) return;
	scrollMove(deltaTime);

	auto boss = world_->findActor("Boss");
	// 中ボスがいないなら、削除する
	if (boss == nullptr) {
		isPlayerIn_ = false;
		world_->setIsMBossStage(false);
		dead();
	}
}

bool BossScrollPoint::isInMBossStage()
{
	return isPlayerIn_;
}

// プレイヤーの位置を計算
void BossScrollPoint::playerPosition(const std::string name)
{
	// すでにスクロールする場合は返す
	if (world_->isMBossStage()) return;
	auto player = world_->findActor(name);
	if (player != nullptr) {
		// 範囲外に居た場合は返す
		auto distance = Vector2(position_ - player->getPosition());
		if (std::abs(distance.x) > CHIPSIZE * 9.5f ||
			std::abs(distance.y) > CHIPSIZE * 5.5f) return;
		// 範囲内にいる処理
		world_->PlayerNotMove(true);
		player_ = player;
		isPlayerIn_ = true;
	}
	else return;
}

// スクロール時の移動
void BossScrollPoint::scrollMove(float deltaTime)
{
	if (world_->isMBossStage()) return;
	// このオブジェクトの位置にスクロールする
	auto player1 = dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody1").get());
	if (player1 == nullptr) return;
	//auto direction = position_ - 
	// 移動方向の設定
	if (prevPlayerDirection_.x == 0.0f)
		prevPlayerDirection_ = Vector2(player1->getPosition() - position_).Normalize();
	auto direction = Vector2::Right;
	if (prevPlayerDirection_.x < 0.0f) direction.x = -1.0f;
	auto scroolPos = position_+ Vector2(CHIPSIZE * 4 * direction.x, CHIPSIZE * 6);
	// 指定の位置に着いた場合は返す
	if (player_->getPosition().x == scroolPos.x) {
		world_->setIsMBossStage(true);
		//world_->PlayerNotMove(false);
		return;
	}
	// プレイヤーを動かす
	auto player2 = dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody2").get());
	if (player2 == nullptr) return;
	// 移動速度
	auto speed = min(250.0f / 60.0f, 
		std::abs(scroolPos.x - player_->getPosition().x) / 60.0f);
	if (speed < 250.0f / 60.0f) {
		world_->setIsMBossStage(true);
		//world_->PlayerNotMove(false);
		return;
	}
	auto velocity = direction * -(speed * 60.0f);
	player1->ForcedMove(velocity);
	player2->ForcedMove(velocity);
	auto playerConnecter = dynamic_cast<PlayerConnector*>(world_->findActor("PlayerConnector").get());
	if (playerConnecter == nullptr) return;
	playerConnecter->ForcedMove(velocity);
	//dynamic_cast<PlayerBody*>(world_->findActor("PlayerBody1").get())->ForcedMove(Vector2(150.0f, 0.0f));
}
