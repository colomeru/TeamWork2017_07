#include "BossEntry.h"
#include "Effect/ImportEffects.h"
#include "../../../../World/IWorld.h"
#include "../../../Base/ActorGroup.h"

BossEntry::BossEntry(
	IWorld * world,
	const Vector2 & position,
	const Vector2 & addPosition,
	const float bodyScale) :
	Actor(world, "BossEntry", position + addPosition,
		CollisionBase(
			const_cast<Vector2&>(position) + 
			const_cast<Vector2&>(addPosition),
			bodyScale)),
	blockTimer_(0.0f),
	isEntry_(false),
	isEntered_(false),
	isBlock_(false),
	direction_(1.0f, -1.0f),
	bossPosition_(position),
	addPosition_(addPosition)
{}

void BossEntry::onUpdate(float deltaTime)
{
	auto addPos = Vector2(
		addPosition_.x * direction_.x,
		addPosition_.y * direction_.y);
	position_ = bossPosition_ + addPos;
	// ブロックされていたら、時間を加算
	if (isBlock_)
		blockTimer_ += deltaTime;
	else blockTimer_ = 0.0f;
	// bool系の更新
	isBlock_ = false;
}

void BossEntry::onDraw() const{}

void BossEntry::onCollide(Actor & actor)
{
	// プレイヤーをつないでいる線に当たったら(口が防がれたら)
	if (actor.getName() == "BodyPoint") {
		isBlock_ = true;
	}
	// 中に入れる状態のみ、プレイヤーの本体に当たる
	if (!isEntry_) return;
	if (actor.getName() == "PlayerBody1Collider" ||
		actor.getName() == "PlayerBody2Collider") {
		isEntered_ = true;
	}
}

void BossEntry::onMessage(EventMessage event, void *){}

// 位置の設定をします
void BossEntry::setBossPosition(const Vector2 & position)
{
	bossPosition_ = position;
}

// 方向を設定します
void BossEntry::setDirection(const Vector2 & direction)
{
	direction_ = direction;
}

// ボスの体内に入れるかを設定します
void BossEntry::setIsEntry(bool isEntry)
{
	isEntry_ = isEntry;
}

// ボスの体内に入ったかを返します
bool BossEntry::isEntered()
{
	return isEntered_;
}

// プレイヤーをつなぐ線に触れているかを返します
bool BossEntry::isBlock()
{
	return isBlock_;
}

// プレイヤーがボスの体内に出たことにします
void BossEntry::letOut()
{
	isEntry_ = false;
	isEntered_ = false;
}
