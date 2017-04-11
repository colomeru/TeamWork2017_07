#include "FloorSearchPoint.h"
#include "../../../ResourceLoader/ResourceLoader.h"
#include "../../Base/ActorGroup.h"
#include"../../Body/CollisionBase.h"
#include "../../../World/IWorld.h"
#include "../../../Math/Math.h"
#include "../../../Define.h"

FloorSearchPoint::FloorSearchPoint(
	IWorld * world, 
	const Vector2& enemyPosition,
	const Vector2& addPosition,
	const Vector2& bodyScale
	) :
	Actor(world, "FSP", enemyPosition + addPosition,
		CollisionBase(
			Vector2((enemyPosition.x + addPosition.x) + bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) + bodyScale.y / 2.0f),
			Vector2((enemyPosition.x + addPosition.x) - bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) + bodyScale.y / 2.0f),
			Vector2((enemyPosition.x + addPosition.x) + bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) - bodyScale.y / 2.0f),
			Vector2((enemyPosition.x + addPosition.x) - bodyScale.x / 2.0f, (enemyPosition.y + addPosition.y) - bodyScale.y / 2.0f)
			)
		),
	turnCount_(0),
	isFloor_(false),
	isGround_(false),
	isGroundBegin_(false),
	direction_(1.0f, 1.0f),
	enemyPosition_(enemyPosition),
	addPosition_(addPosition),
	floorPosition_(Vector2::Zero)
{
}

FloorSearchPoint::FloorSearchPoint(
	IWorld * world,
	const Vector2 & enemyPosition,
	const Vector2 & addPosition,
	const float radius) :
	Actor(world, "FSP", enemyPosition + addPosition,
		CollisionBase(position_, radius)),
	turnCount_(0),
	isFloor_(false),
	isGround_(false),
	isGroundBegin_(false),
	direction_(1.0f, 1.0f),
	enemyPosition_(enemyPosition),
	addPosition_(addPosition),
	floorPosition_(Vector2::Zero)
{
}

void FloorSearchPoint::onUpdate(float deltaTime)
{
	auto addPos = Vector2(
		addPosition_.x * direction_.x,
		addPosition_.y * direction_.y);
	position_ = enemyPosition_ + addPos;

	isFloor_ = false;
	isGround_ = false;
	isGroundBegin_ = false;
}

void FloorSearchPoint::onDraw() const{
	//auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	//vec3Pos = vec3Pos * inv_;
	////// 画像の表示
	//DrawGraph(
	//	vec3Pos.x, vec3Pos.y,
	//	ResourceLoader::GetInstance().getTextureID(TextureID::ENEMY_NEEDLE_TEX),
	//	true);
}

void FloorSearchPoint::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	// プレイヤー関連のオブジェクトに当たっているなら
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	// 床に当たっていた、振り向き回数をリセット
	if (getFloorName != NULL || actorName == "Door") {
		turnCount_ = 0;
		isGround_ = true;
		isFloor_ = true;
		return;
	}
}

void FloorSearchPoint::onMessage(EventMessage event, void *){}

// 位置の設定
void FloorSearchPoint::setPosition(const Vector2& position)
{
	enemyPosition_ = position;
}

// 方向の設定
void FloorSearchPoint::setDirection(const Vector2& direction)
{
	direction_ = direction;
}

// 床と当たったかを返します
bool FloorSearchPoint::isFloor()
{
	return isFloor_;
}

bool FloorSearchPoint::isGround()
{
	return isGround_;
}

// 床と当たった場所を返します
Vector2 FloorSearchPoint::getFloorPosition()
{
	return floorPosition_;
}
