#include "Prickle.h"
#include "../../../../ResourceLoader/ResourceLoader.h"
#include "../../../Base/ActorGroup.h"
#include"../../../Body/CollisionBase.h"
#include "../../../../World/IWorld.h"
#include "../../../../Math/Math.h"

Prickle::Prickle(
	IWorld * world,
	const Vector2& enemyPosition,
	const Vector2& addPosition,
	const Vector2& bodyScale) :
	Actor(world, "Prickle", enemyPosition + addPosition, CollisionBase(
		Vector2((enemyPosition.x + addPosition.x) + bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) + bodyScale.y / 2.0f),
		Vector2((enemyPosition.x + addPosition.x) - bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) + bodyScale.y / 2.0f),
		Vector2((enemyPosition.x + addPosition.x) + bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) - bodyScale.y / 2.0f),
		Vector2((enemyPosition.x + addPosition.x) - bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) - bodyScale.y / 2.0f)
		)),
	ap_(10),
	direction_(Vector2(1.0f, 1.0f)),
	enemyPosition_(enemyPosition),
	addPosition_(addPosition)
{
}

void Prickle::onUpdate(float deltaTime)
{
	auto addPos = Vector2(
		addPosition_.x * direction_.x,
		addPosition_.y * direction_.y);
	position_ = enemyPosition_ + addPos;
}

void Prickle::onDraw() const{}

void Prickle::onCollide(Actor & actor){}

void Prickle::onMessage(EventMessage event, void *){}

// 位置を入れます
void Prickle::setEnemyPosition(const Vector2 & position)
{
	enemyPosition_ = position;
}

// 追加位置を入れます
void Prickle::setAddPosition(const Vector2 & position)
{
	addPosition_ = position;
}

// 方向を入れます
void Prickle::setDirection(const Vector2 & direction)
{
	direction_ = direction;
}

// プレイヤーに与えるダメージ量を返します
int Prickle::getAP()
{
	return ap_;
}
