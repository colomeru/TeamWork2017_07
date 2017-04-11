#include "ScatterWingEffect.h"
#include "BossWingEffect.h"
#include "../../../../Base/ActorGroup.h"
#include "../../../../../World/IWorld.h"
#include <random>

ScatterWingEffect::ScatterWingEffect(
	IWorld * world,
	const Vector2 & position) :
	Actor(world, "ScatterWingEffect", position, CollisionBase())
{
	auto range = 100;
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	// 初期Seed値を渡す
	std::mt19937 mt(random());
	// 範囲の指定(int型)
	std::uniform_int_distribution<> cRange(-range, range);
	std::uniform_int_distribution<> scale(8, 14);
	// 羽エフェクトの生成
	for (int i = 0; i != 6; i++) {
		auto wingEffect = std::make_shared<BossWingEffect>(
			world_, position_ + Vector2((float)cRange(mt), cRange(mt) / 10.0f), 
			(float)scale(mt));
		world_->addActor(ActorGroup::EnemyBullet, wingEffect);
	}
}

void ScatterWingEffect::onUpdate(float deltaTime){}

void ScatterWingEffect::onDraw() const{}

void ScatterWingEffect::onCollide(Actor & actor){}
