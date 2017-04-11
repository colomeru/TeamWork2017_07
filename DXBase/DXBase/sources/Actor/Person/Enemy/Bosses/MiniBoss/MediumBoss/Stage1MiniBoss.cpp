#include "Stage1MiniBoss.h"
#include "../ImportAnimationNumber.h"
#include "../../../Bullet/WingBullet.h"
#include "../../../../../../World/IWorld.h"
#include "../../../../../../Scene/Base/SceneDataKeeper.h"
#include "../../../../../../Define.h"
#include "../../../../../Item/MoveItem.h"
#include "../../../../../Base/ActorGroup.h"

Stage1MiniBoss::Stage1MiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const float bodyScale) :
	MediumBoss(world, position, bodyScale),
	attackCount_(0),
	isAttack_(false),
	direction_(Vector2::Right)
{
	// 
	wingDegrees.clear();
	auto degree = 30.0f;
	wingDegrees[0] = 0.0f;
	wingDegrees[1] = degree / 2;
	wingDegrees[2] = -degree / 2;
	wingDegrees[3] = degree;
	wingDegrees[4] = -degree;
}

void Stage1MiniBoss::idel(float deltaTime)
{
	if (stateTimer_ < 2.0f) return;
	changeState(State::Attack, WING_ATTACK_NUMBER);
}

void Stage1MiniBoss::move(float deltaTime)
{
}

void Stage1MiniBoss::attack(float deltaTime)
{
	// çUåÇÇÇµÇƒÇ¢Ç»Ç©Ç¡ÇΩÇÁÅAâHçUåÇÇçsÇ§
	if (!isAttack_) {
		if (animation_.getFrame() < 25) return;
		wingAttack();
		attackCount_++;
		isAttack_ = true;
	}

	if (stateTimer_ < 3.0f) return;
	// âHçUåÇÇ™ÇRâÒèIÇÌÇ¡ÇΩÇÁÅAë“ã@èÛë‘Ç…ëJà⁄
	if (attackCount_ >= 3) {
		changeState(State::Idel, WAIT_NUMBER);
		attackCount_ = 0;
	}
	// çUåÇèÛë‘ÇÃÉäÉZÉbÉg
	isAttack_ = false;
	animation_.initAnimeTime();
	animation_.setIsLoop(true);
	stateTimer_ = 0.0f;
}

// âHçUåÇ
void Stage1MiniBoss::wingAttack()
{
	auto speed = 10.0f;
	auto size = wingDegrees.size();
	//std::uniform_int_distribution<> count(0, size - 1);
	auto value = getRandomInt(0, size- 1);
	auto addDegree = 0.0f;
	// å¸Ç¢ÇƒÇ¢ÇÈï˚å¸Ç≈ÅAâ¡éZÇ∑ÇÈäpìxÇïœÇ¶ÇÈ
	if (direction_.x >= 0) addDegree = 180.0f;
	// ê∂ê¨
	for (auto i = 0; i != size; i++) {
		auto degree = wingDegrees[(i + value) % size] + addDegree;
		if (i == 0) {
			// êØÇÃê∂ê¨
			auto direction = Vector2(
				MathHelper::Cos(degree),
				MathHelper::Sin(degree)).Normalize();
			auto obj = std::make_shared<MoveItem>(
				world_, position_, direction, speed);
			world_->addActor(ActorGroup::Item, obj);
		}
		else {
			// âHÉoÉåÉbÉgÇÃê∂ê¨
			auto obj = std::make_shared<WingBullet>(
				world_, position_, degree, speed);
			world_->addActor(ActorGroup::Enemy, obj);
		}
	}
	animation_.changeAnimation(WING_ATTACK_NUMBER);
	animation_.setIsLoop(false);
}
