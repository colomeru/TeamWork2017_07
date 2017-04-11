#include "Stage2MiniBoss.h"
#include "../ImportAnimationNumber.h"
#include "../../Effect/PiyoriEffect.h"
#include "../../../../../Base/ActorGroup.h"
#include "../../../../../../World/IWorld.h"
#include "../../../../../../Define.h"

Stage2MiniBoss::Stage2MiniBoss(
	IWorld * world, 
	const Vector2 & position, 
	const float bodyScale) : 
	MediumBoss(world, position, bodyScale),
	movePositionCount_(0.0f),
	effectCreateTimer_(0.0f),
	isEffectCreate_(false),
	isReturn_(false),
	prevAttackPosition_(position_)
{
	direction_ = Vector2::Left;
	color_ = Vector3(180.0f, 250.0f, 0.0f);
	stars_.clear();
}

void Stage2MiniBoss::idel(float deltaTime)
{
	auto speed = 6.0f;
	auto addPos = speed * direction_.x * (deltaTime * 60.0f);
	position_.x += addPos;
	movePositionCount_ += std::abs(addPos);
	// 一定距離移動をしたら、反対方向に移動
	if (movePositionCount_ >= CHIPSIZE * 8) {
		direction_.x *= -1.0f;
		movePositionCount_ = 0.0f;
	}
	animation_.changeDirType(direction_.x);
	// 一定時間経過で、攻撃状態に遷移
	if (stateTimer_ < 2.0f) return;
	changeState(State::Attack, WALL_ATTACK_NUMBER);
	prevAttackPosition_ = position_;
	animation_.setIsLoop(false);
	// SEの再生
	PlaySoundMem(
		ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_WALLATTACK),
		DX_PLAYTYPE_BACK);
	auto player = world_->findActor("PlayerBody1");
	if (player == nullptr) return;
	// プレイヤーが居た場所との距離を計算
	prevPlayerDirection_ = Vector2(player->getPosition() - position_).Normalize();
	// プレイヤーの方向を向く
	auto angle = 
		std::atan2(prevPlayerDirection_.y, prevPlayerDirection_.x) * 
		180.0f / MathHelper::Pi;
	degree_ = angle + 180.0f;
}

void Stage2MiniBoss::move(float deltaTime)
{
	// ぴより行動
	if (!isReturn_) piyoriMove(deltaTime);
	else returnMove(deltaTime);
}

void Stage2MiniBoss::attack(float deltaTime)
{
	auto speed = 18.0f;
	position_ += speed * prevPlayerDirection_ * (deltaTime * 60.0f);
}

void Stage2MiniBoss::deadMove(float deltaTime)
{
	// スターの削除
	if (stars_.size() != 0) {
		for (auto i = stars_.begin(); i != stars_.end(); i++) {
			auto star = *i;
			star->dead();
		}
		stars_.clear();
	}
	MediumBoss::deadMove(deltaTime);
}

// 床に当たった時の処理
void Stage2MiniBoss::floorHit()
{
	// 攻撃状態のときに、壁に当たったらぴより状態に遷移
	if (state_ != State::Attack) return;
	changeState(State::Move, PIYORI_NUMBER);
	degree_ = 90.0f;
	animation_.setIsLoop(true);
}

// ぴより行動
void Stage2MiniBoss::piyoriMove(float deltaTime)
{
	auto dir = direction_;
	dir.y = 1.0f;
	// 星の生成
	if (isEffectCreate_ && (int)effectCreateTimer_ % 50 <= 24 && stars_.size() < 6) {
		auto star = std::make_shared<PiyoriEffect>(
			world_, position_, 0.5f);
		world_->addActor(ActorGroup::Effect, star);
		stars_.push_back(star);
		isEffectCreate_ = false;
	}
	else if ((int)effectCreateTimer_ % 50 > 24) {
		isEffectCreate_ = true;
	}
	effectCreateTimer_ += deltaTime * 60.0f;
	// ぴよりエフェクトの位置
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto addPos = Vector2::Up * -50.0f;
		i->get()->position_ = position_ + addPos;
	}
	// 重力
	if (!isGround_) {
		position_.y += 9.8f * (deltaTime * 60.0f);
	}
	// 一定時間経過で待機状態に遷移
	if (stateTimer_ < 5.0f) return;
	isReturn_ = true;
	isEffectCreate_ = true;
	animation_.changeAnimation(WAIT_NUMBER);
	// スターの削除
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto star = *i;
		star->dead();
	}
	stars_.clear();
	effectCreateTimer_ = 0.0f;
}

// 戻り行動
void Stage2MiniBoss::returnMove(float deltaTime)
{
	auto speed = 4.0f;
	auto distance = prevAttackPosition_ - position_;
	if (speed > distance.Length()) {
		position_ = prevAttackPosition_;
		changeState(State::Idel, WAIT_NUMBER);
		isReturn_ = false;
	}
	else position_ += speed * distance.Normalize() * (deltaTime * 60.0f);
	animation_.changeDirType(distance.Normalize().x);
}
