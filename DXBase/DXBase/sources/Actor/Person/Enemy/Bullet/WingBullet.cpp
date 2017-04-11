#include "WingBullet.h"
#include "../../../../ResourceLoader/ResourceLoader.h"

WingBullet::WingBullet(
	IWorld * world, 
	const Vector2 & position, 
	const float degree, 
	const float speed,
	const float bodyScale) : 
	Actor(world, "WingBulletEnemy", position, 
		CollisionBase(const_cast<Vector2&>(position), bodyScale)),
	speed_(speed),
	degree_(degree),
	direction_(
		Vector2(MathHelper::Cos(degree), 
		MathHelper::Sin(degree)).Normalize()),
	isFloorHit_(false)
{
}

void WingBullet::onUpdate(float deltaTime)
{
	if (isFloorHit_) {
		// ìßñæèàóù
		alpha_ -= 255 * deltaTime;
		alpha_ = max(alpha_, 0.0f);
		if (alpha_ <= 0.0f)
			dead();
	}
	else position_ += direction_ * speed_ * (deltaTime * 60.0f);
}

void WingBullet::onDraw() const
{
	auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	vec3Pos = vec3Pos * inv_;
	auto pos = Vector2(vec3Pos.x, vec3Pos.y);
	auto scale = 16.0f;
	// âÊëúÇÃï\é¶
	// ÉøÉuÉåÉìÉhÇÃê›íË
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	//SetDrawBright((int)color_.x, (int)color_.y, (int)color_.z);
	DrawRotaGraph3(
		(int)pos.x, (int)pos.y,
		(int)body_.GetCircle().getRadius(), (int)body_.GetCircle().getRadius(),
		scale / body_.GetCircle().getRadius(), 
		scale / body_.GetCircle().getRadius(), 
		MathHelper::ToRadians(degree_ + 160.0f),
		ResourceLoader::GetInstance().getTextureID(TextureID::BOSS_WING_TEX), 1, direction_.x < 0);
	//SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void WingBullet::onCollide(Actor & actor)
{
	if (isFloorHit_) return;
	auto actorName = actor.getName();
	auto getPlayerName = strstr(actorName.c_str(), "Player");
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	if (getFloorName != NULL) {
		isFloorHit_ = true;
		return;
	}

	if (getPlayerName != NULL) {
		dead();
		return;
	}
}
