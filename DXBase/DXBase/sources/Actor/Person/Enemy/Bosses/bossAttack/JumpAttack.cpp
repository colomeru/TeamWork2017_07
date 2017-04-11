#include "JumpAttack.h"
#include "../Effect/ImportEffects.h"
#include "../../Rock.h"

JumpAttack::JumpAttack() :
	BossAttack(nullptr, Vector2::Zero),
	degrees_(0.0f),
	jumpPower_(0.0f),
	initJumpPower_(jumpPower_),
	recastTimer_(0.0f),
	initRecastTimer_(recastTimer_),
	isJump_(true),
	isFirstJump_(true),
	isJumpEnd_(true),
	isIdel_(false),
	isRockCreate_(true),
	speed_(Vector2::Zero),
	prevPlayerDistance_(Vector2::Zero)
{}

JumpAttack::JumpAttack(IWorld* world, const Vector2& position) :
	BossAttack(world, position),
	degrees_(60.0f),
	jumpPower_(15.0f),
	initJumpPower_(jumpPower_),
	recastTimer_(0.5f),
	initRecastTimer_(recastTimer_),
	isJump_(false),
	isFirstJump_(false),
	isJumpEnd_(false),
	isIdel_(false),
	isRockCreate_(false),
	speed_(Vector2::One * 2.0f),
	otherName_(""),
	prevOtherName_(""),
	prevPlayerDistance_(Vector2::One)
{
	isBodyHit_ = false;
	isAttackHit_ = true;
	animeNum_ = JUMP_UP_NUMBER;
}

// 攻撃
void JumpAttack::attack(float deltaTime)
{

	if (collideObj_ != nullptr)
		otherName_ = collideObj_->getName();
	if (otherName_ == "PlayerAttackCollider")
		isAttackHit_ = false;
	if (!isFirstJump_) {
		// 方向の設定
		auto distance = pPosition_ - position_;
		// 方向の値を代入
		if (distance.x < 0)
			pDirection_.x = -1;
		else if (distance.x > 0)
			pDirection_.x = 1;
		direction_ = pDirection_;
		// 速度の決定
		speed_.y = (float)getRandomInt(10, 20) * 0.1f;
		// SEの再生
		PlaySoundMem(
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_JUMP),
			DX_PLAYTYPE_BACK);
		//setJumpSpeed();
		isFirstJump_ = true;
	}
	// ジャンプ攻撃中に床に接地したら、リキャストに移行
	if (isJump_ && isGround_ && timer_ > 0.3f) {
		isIdel_ = true;
		isBodyHit_ = true;
		auto addPos = Vector2::Left * 40.0f;
		if (!isRockCreate_) {
			world_->addActor(ActorGroup::Effect,
				std::make_shared<BokoEffect>(world_, position_ + addPos, 255, 3.0f));
			// 岩の生成
			createRock();
			// 着地時に攻撃判定追加
			PlaySoundMem(
				ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_CHAKUCHI),
				DX_PLAYTYPE_BACK);
		}
		isRockCreate_ = true;
	}
	// ジャンプが終了したら、リキャストタイムを減算する
	if (isIdel_) {
		recastTimer_ -= deltaTime;
		// リキャスト時間が 0 になったら、ジャンプ攻撃終了
		if (recastTimer_ > 0.0f) return;
		jumpPower_ = initJumpPower_;
		isJump_ = false;
		isJumpEnd_ = true;
		timer_ = 0.0f;
		// 方向の設定
		auto distance = pPosition_.x - position_.x;
		// 方向の値を代入
		if (distance < 0)
			pDirection_.x = -1;
		else if (distance >= 0)
			pDirection_.x = 1;
		direction_ = pDirection_;
		return;
	}
	// リキャスト状態でなければジャンプ攻撃
	if (recastTimer_ == initRecastTimer_)
		jump(deltaTime);
	// 前回衝突したオブジェクトの名前を更新
	prevOtherName_ = otherName_;
	otherName_ = "";
	collideObj_ = nullptr;
}

// ジャンプ攻撃
void JumpAttack::jump(float deltaTime)
{
	if (isBottom_)
		jumpPower_ = 0;
	// 移動
	auto addPos = Vector2(
		-std::cos(degrees_) * speed_.x * pDirection_.x * (deltaTime * 60.0f),
		(-jumpPower_ / 10 + timer_ * speed_.y) * 9.8f * 
		((deltaTime * 60.0f)* speed_.y));
	position_ += addPos;
	// 移動量が - なら上、+ なら下のアニメーションにする
	if (addPos.y <= 0) animeNum_ = JUMP_UP_NUMBER;
	else animeNum_ = JUMP_DOWN_NUMBER;
	// ジャンプをした
	isJump_ = true;
	isAttackStart_ = true;
	// プレイヤーに当たってもダメージなし
	isBodyHit_ = false;
}

// 攻撃行動のリフレッシュを行います
void JumpAttack::Refresh()
{
	BossAttack::Refresh();
	isJump_ = false;
	isFirstJump_ = false;
	isJumpEnd_ = false;
	isIdel_ = false;
	isRockCreate_ = false;
	isBodyHit_ = true;
	jumpPower_ = initJumpPower_;
	recastTimer_ = initRecastTimer_;
	animeNum_ = JUMP_UP_NUMBER;
}

// 岩の生成
void JumpAttack::createRock()
{
	// 範囲の指定(int型)
	int size = static_cast<int>(CHIPSIZE);
	// 岩の生成
	for (int i = 0; i != 3; i++) {
		// Xの生成位置を入れる
		world_->addActor(ActorGroup::Enemy,
			std::make_shared<Rock>(world_,
				Vector2((float)getRandomInt(size * 2, size * 18), 200.0f)));
	}
}
