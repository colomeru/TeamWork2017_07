#include "PowerUpEffect.h"

PowerUpEffect::PowerUpEffect(
	IWorld * world,
	const std::string name,
	const Vector2 & position,
	const Vector2 & size,
	const int id,
	const AnimationID animeID) : 
	Actor(world, name, position, CollisionBase()),
	size_(size),
	scale_(Vector2::One),
	animation_(EnemyAnimation2D())
{
	animation_.addAnimation(
		id, 
		ResourceLoader::GetInstance().getAnimationIDs(animeID));
	animation_.changeAnimation(id);
	animation_.setIsLoop(false);
}

void PowerUpEffect::onUpdate(float deltaTime)
{
	animation_.update(deltaTime);
	if (animation_.isEndAnimation() && !animation_.isLoop_)
		dead();
}

void PowerUpEffect::onDraw() const
{
	auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	vec3Pos = vec3Pos * inv_;
	// アニメーションの描画
	auto pos = Vector2(vec3Pos.x, vec3Pos.y);
	// αブレンドの設定
	animation_.draw(pos, size_ / 2, scale_);
}
