#include "StarPowerUpEffect.h"
#include "PowerUpFontEffect.h"
#include "../../Base/ActorGroup.h"
#include "../../../World/IWorld.h"

StarPowerUpEffect::StarPowerUpEffect(
	IWorld * world, 
	const Vector2 & position) :
	PowerUpEffect(world, "StarPowerUpEffect", position, Vector2::One * 256,
		STAR_POWER_UP_NUMBER, AnimationID::PLAYER_EFFECT_STAR_POWER_UP),
	isReverse_(false),
	fontEffect_(nullptr)
{
	scale_ = Vector2(2.25f, 2.0f);
	auto effect = std::make_shared<PowerUpFontEffect>(world, position - Vector2::Up * 200.0f);
	world_->addActor(ActorGroup::Effect, effect);
	fontEffect_ = effect.get();
}

void StarPowerUpEffect::onUpdate(float deltaTime)
{
	animation_.update(deltaTime);
	if (animation_.getFrame() > 35 && fontEffect_ != nullptr) {
		//animation_.setIsReverse(true);
		if (!isReverse_) {
			//fontEffect_->getAnimation().setIsLoop(true);
			fontEffect_->animaReverse();
			//fontEffect_->getAnimation().setIsLoop(false);
			isReverse_ = true;
		}
		if (fontEffect_->isEnd()) {
			fontEffect_->dead();
			fontEffect_ = nullptr;
		}
	}
	else if (animation_.isEndAnimation()) {
		dead();
	}
}
