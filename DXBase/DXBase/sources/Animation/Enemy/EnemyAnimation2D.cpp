#include "EnemyAnimation2D.h"
#include <random>

EnemyAnimation2D::EnemyAnimation2D() : 
	prevFrame_(0),
	isLoop_(true),
	isStop_(false),
	isTurn_(false),
	isReverse_(false){}

EnemyAnimation2D::~EnemyAnimation2D()
{
	sprites_.clear();
}

void EnemyAnimation2D::update(float deltaTime)
{
	preMotion();
	// アニメーションのタイムが一周したら、止める
	if (isStop_)return;
	// 更新
	if (curr_anim_ >= 15 || curr_anim_ <= -1) return;
	frame_ = static_cast<int>(timer_) % sprites_[curr_anim_].size();
	id_ = sprites_[curr_anim_][frame_];
	auto timer = deltaTime * curr_speed_ * 60.0f / sprites_[curr_anim_].size() * 10;
	// 逆再生するならば、値を反転する
	if (isReverse_)
		timer *= -1.0f;
	timer_ += timer;
	// ループしないなら
	if (!isLoop_) {
		// アニメーションのタイムが一周したら、止める
		if (!isReverse_) {
			if (timer_ >= sprites_[curr_anim_].size() - 2) {
				frame_ = static_cast<unsigned int>(sprites_[curr_anim_].size() - 1);
				timer_ = (float)sprites_[curr_anim_].size() - 1;
				isStop_ = true;
			}
		}
		else {
			if (timer_ <= 1) {
				frame_ = 0;
				timer_ = 0;
				isStop_ = true;
			}
		}
	}
	isBeginTurn_ = false;
	if (isTurn_)
		isBeginTurn_ = true;
	isTurn_ = false;
	// ループ時の更新
	// フレームの更新
	prevFrame_ = frame_;
}

// アニメーションの追加
void EnemyAnimation2D::addAnimation(int id, const std::vector<int>& anims)
{
	add_anim(id, anims);
}

// アニメーションの変更
void EnemyAnimation2D::changeAnimation(int id, float speed)
{
	isLoop_ = true;
	isStop_ = false;
	// 親の変更を使う
	change_param(id, speed);
}

// アニメーションのが終わったかを返します
bool EnemyAnimation2D::isEndAnimation()
{
	return end_anim();
}

// 振り向きアニメーションを行って、画像を反転します
void EnemyAnimation2D::turnAnimation(int id, float direction)
{
	auto type = ActionType::Right;
	// アクションタイプの取得
	if (direction >= 0)
		type = ActionType::Left;
	change_dir_type(id, type);
}

// アニメーションを振り向きアニメーションを行わずに画像を反転します
void EnemyAnimation2D::changeDirType(float direction)
{
	auto type = ActionType::Right;
	// アクションタイプの取得
	if (direction >= 0)
		type = ActionType::Left;
	if (type_ == type) return;
	type_ = type;
	type_stock_ = type;
	isTurn_ = true;
}

// アニメーションの時間を初期化します
void EnemyAnimation2D::initAnimeTime()
{
	frame_ = 0;
	timer_ = 0;
}

// アニメーションの再生速度を変更します
void EnemyAnimation2D::setSpeed(float speed)
{
	curr_speed_ = norm_speed_ = speed;
}

// アニメーションをループさせるかを設定します
void EnemyAnimation2D::setIsLoop(bool isLoop)
{
	isLoop_ = isLoop;
	if (isLoop_)
		isStop_ = false;
}

// 逆再生するかを設定します
void EnemyAnimation2D::setIsReverse(bool isReverse)
{
	isReverse_ = isReverse;
	isStop_ = false;
}

void EnemyAnimation2D::preMotion()
{
	if (turn_anim_ != -1 && end_anim())
		isTurn_ = false;
	back_to_pre_motion();
}

void EnemyAnimation2D::stopAnime()
{
	isStop_ = true;
}

// アニメーションのタイムをランダムにします
void EnemyAnimation2D::timeRandom()
{
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	// 初期Seed値を渡す
	std::mt19937 mt(random());
	// 範囲の指定(int型)
	std::uniform_int_distribution<> rFrame(0, sprites_[curr_anim_].size() - 1);
	//return count(mt);
	frame_ = static_cast<unsigned int>(rFrame(mt));
	timer_ = (float)rFrame(mt);
}

// アニメーションの向きを変えたかを返します(1f)
bool EnemyAnimation2D::isBeginTurn()
{
	return isBeginTurn_;
}

// 現在の再生フレームを取得します
unsigned int EnemyAnimation2D::getFrame()
{
	return frame_;
}
