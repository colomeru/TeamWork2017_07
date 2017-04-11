#include "WingAttackMiniBoss.h"
#include "ImportAnimationNumber.h"
#include "../../Bullet/WingBullet.h"
#include "../../../../Base/ActorGroup.h"
#include "../../../../../World/IWorld.h"
#include "../../../../../Define.h"
#include "../../../../../Scene/Base/SceneDataKeeper.h"
#include "../../../../Item/Items.h"

WingAttackMiniBoss::WingAttackMiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const float bodyScale) : 
	FighterMiniBoss(world, position, bodyScale),
	attackCount_(0),
	isAttack_(false),
	direction_(Vector2::Right),
	mt_(std::mt19937())
{
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	// 初期Seed値を渡す
	std::mt19937 mt(random());
	mt_ = mt;

	wingDegrees.clear();
	auto degree = 30.0f;
	wingDegrees[0] = 0.0f;
	wingDegrees[1] = degree / 2;
	wingDegrees[2] = -degree / 2;
	wingDegrees[3] = degree;
	wingDegrees[4] = -degree;
}

void WingAttackMiniBoss::idel(float deltaTime)
{
	if (stateTimer_ < 2.0f) return;
	changeState(State::Attack, WING_ATTACK_NUMBER);
}

void WingAttackMiniBoss::move(float deltaTime)
{
}

void WingAttackMiniBoss::attack(float deltaTime)
{
	if (!isAttack_) {
		if (animation_.getFrame() < 25) return;
		wingAttack();
		attackCount_++;
		isAttack_ = true;
	}

	if (stateTimer_ < 3.0f) return;
	if (attackCount_ >= 3) {
		changeState(State::Idel, WAIT_NUMBER);
		attackCount_ = 0;
	}
	isAttack_ = false;
	animation_.initAnimeTime();
	stateTimer_ = 0.0f;
}

void WingAttackMiniBoss::deadMove(float deltaTime)
{
}

void WingAttackMiniBoss::addAnimation()
{
}

// 羽攻撃
void WingAttackMiniBoss::wingAttack()
{
	auto size = wingDegrees.size();
	std::uniform_int_distribution<> count(0, size - 1);
	auto value = count(mt_);
	auto addDegree = 0.0f;
	// 向いている方向で、加算する角度を変える
	if (direction_.x >= 0) addDegree = 180.0f;
	// 生成
	for (auto i = 0; i != size; i++) {
		auto degree = wingDegrees[i + value % size] + addDegree;
		if (i == 0) {
			auto obj = std::make_shared<Items>(world_, position_);
			world_->addActor(ActorGroup::Item, obj);
		}
		else {
			auto obj = std::make_shared<WingBullet>(world_, position_, degree);
			world_->addActor(ActorGroup::Enemy, obj);
		}		
	}
	animation_.changeAnimation(WING_ATTACK_NUMBER);
	animation_.setIsLoop(false);
}
