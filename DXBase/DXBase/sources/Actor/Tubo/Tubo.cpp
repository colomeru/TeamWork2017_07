#include "Tubo.h"
#include "../../Game/Time.h"

#include "../../ResourceLoader/ResourceLoader.h"

#include "../../Define.h"

Tubo::Tubo(IWorld * world, const Vector2 & position) :
	Actor(world, "Tubo", position, CollisionBase(
		Vector2{ position.x ,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) }
		)),
	mVelo(Vector2::Zero),
	mDownFlag(false),
	timer_(0)
{

}

Tubo::~Tubo()
{
}

void Tubo::onUpdate(float deltaTime)
{

	if (mDownFlag) {
		mVelo += Vector2(0.0f, 1.0f)*Time::GetInstance().deltaTime();
		position_ += mVelo;

		if (timer_ >= 5.0f)return;
		timer_ += deltaTime * 2;
		if (timer_ >= 5.0f) {
			world_->setEntry(false, true);
		}
	}


	//mVelo = Vector2::Clamp(mVelo, Vector2(0, 0), Vector2(0, 10));



}

void Tubo::onDraw() const{
	body_.draw(ResourceLoader::GetInstance().getTextureID(TextureID::VASE_TEX), inv_);
	//body_.draw(inv_);
}

void Tubo::onCollide(Actor & other)
{
	if (other.getName() == "PlayerBody1"||
		other.getName() == "PlayerBody2") {
		if (timer_ > 0.0f)return;

		auto pos = other.body_.GetCircle().previousPosition_;

		auto t_left  = body_.GetBox().component_.point[0];
		auto t_right = body_.GetBox().component_.point[1];
		auto b_left  = body_.GetBox().component_.point[2];
		auto b_right = body_.GetBox().component_.point[3];

		auto top = Vector2::Cross((t_left - t_right).Normalize(), (pos - t_right));
		auto right = Vector2::Cross((t_right - b_right).Normalize(), (pos - b_right));
		auto bottom = Vector2::Cross((b_right - b_left).Normalize(), (pos - b_left));
		auto left = Vector2::Cross((b_left - t_left).Normalize(), (pos - t_left));

		if (top >= 0 && left <= 0 && right <= 0) {
			world_->setEntry(true, false);
			mDownFlag = true;
		}
	}
	//if (other.getName() == "MovelessFloorBreak") {
	//	////’x‚­‚È‚é
	//	mVelo = Vector2::Zero;
	//}
	if (other.getName() == "MovelessFloor") {
		mDownFlag = false;
	}
}
