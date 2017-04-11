#include "StarEffect.h"
#include "../../World/IWorld.h"
#include "../../ResourceLoader/ResourceLoader.h"

StarEffect::StarEffect(
	IWorld * world, 
	const Vector2 & position,
	const Vector2 & startPosition,
	const Vector2 & movePosition,
	const Vector2 & rotatePosition,
	const float timer) :
	Actor(world, "StarEffect", position, CollisionBase()),
	speed_(15.0f),
	pMoveSpeed_(speed_),
	scale_(Vector2::One * 2),
	idelTimer_(timer),
	isIdel_(true),
	isStart_(true),
	startPosition_(startPosition),
	movePosition_(movePosition),
	rotatePosition_(rotatePosition),
	degree_(0.0f),
	texDegree_(0.0f),
	stateTimer_(0.0f),
	state_(State::StartIdel),
	animation_(EnemyAnimation2D())
{
	auto id = 0;
	animation_.addAnimation(id,
		ResourceLoader::GetInstance().getAnimationIDs(AnimationID::ITEM_ANM));
	animation_.changeAnimation(id);
}

void StarEffect::onUpdate(float deltaTime)
{
	player_ = world_->findActor("Player");
	if (player_ == nullptr)return;

	// 状態の更新
	switch (state_)
	{
	case State::StartIdel : startIdel(deltaTime); break;
	case State::Idel: idel(deltaTime); break;
	case State::PositionMove: positionMove(deltaTime); break;
	case State::RotateMove: rotateMove(deltaTime); break;
	case State::Imbide: imbide(deltaTime); break;
	case State::Stop: stop(); break;
	}

	animation_.update(deltaTime);
	stateTimer_ += deltaTime;
}

void StarEffect::onDraw() const
{
	Vector2 origin = 
		Vector2((ResourceLoader::GetInstance().GetTextureSize(AnimationID::ITEM_ANM)) / 2);
	animation_.draw(position_, origin, scale_, texDegree_);
}

void StarEffect::changeState(State state)
{
	if (state_ == state) return;
	state_ = state;
	stateTimer_ = 0.0f;
}

void StarEffect::startIdel(const float deltaTime)
{
	// 目的地と自分の方向を出す
	//position_ = startPosition_;
	auto distance = startPosition_ - position_;
	// 目的地と速度の差が速度以下なら、位置を変える
	if (distance.Length() < speed_) {
		position_ = startPosition_;
	}
	else
		position_ += Vector2::Normalize(distance)
		* speed_ * (deltaTime * 60.0f);
	if (position_.x == startPosition_.x && position_.y == startPosition_.y) {
		changeState(State::Idel);
		auto se = ResourceLoader::GetInstance().getSoundID(SoundID::SE_GETCOIN);
		ChangeNextPlayVolumeSoundMem(180, se);
		PlaySoundMem(se, DX_PLAYTYPE_BACK);
		isStart_ = false;
	}
}

void StarEffect::idel(const float deltaTime)
{
	//isStart_ = false;
	if (isIdel_) return;
	idelTimer_ = max(idelTimer_ - deltaTime, 0.0f);
	if (idelTimer_ > 0.0f) return;
	changeState(State::PositionMove);
	auto distance = movePosition_ - position_;
	pMoveSpeed_ = distance.Length() / 30.0f;
}

void StarEffect::positionMove(float deltaTime)
{
	// 目的地と自分の方向を出す
	auto distance = movePosition_ - position_;
	// 目的地と速度の差が速度以下なら、位置を変える
	if (distance.Length() < pMoveSpeed_) {
		position_ = movePosition_;
	}
	else
		position_ += Vector2::Normalize(distance)
		* pMoveSpeed_ * (deltaTime * 60.0f); 
	if (position_.x == movePosition_.x && position_.y && movePosition_.y)
		changeState(State::RotateMove);
}

void StarEffect::rotateMove(float deltaTime)
{
	degree_ += deltaTime * (speed_ / 2);
	texDegree_ += 10.0f * deltaTime * 60.0f;
	position_.x += speed_ * cosf(degree_) * 2.0f * (deltaTime * 60.0f);
	position_.y += speed_ * sinf(degree_) * (deltaTime * 60.0f);
}

void StarEffect::imbide(float deltaTime)
{
	degree_ += deltaTime * (speed_ / 2);
	texDegree_ += 10.0f * deltaTime * 60.0f;
	scale_ -= Vector2::One * 4.0f * deltaTime;
	scale_.x = max(scale_.x, 0.0f);
	scale_.y = max(scale_.y, 0.0f);
	/*Vector2 plusPosition;
	Vector2 mathPosition = Vector2::One;

	plusPosition.x =
		mathPosition.x * cosf(degree_)
		- mathPosition.y * sinf(degree_);
	plusPosition.y =
		mathPosition.x * sinf(degree_)
		+ mathPosition.y * cosf(degree_);
	plusPosition /= 2;
	plusPosition.x = plusPosition.x * 2;*/

	// 目的地と自分の方向を出す
	auto distance = movePosition_ - position_;
	auto dir = Vector2::Normalize(distance);
	auto addDir = Vector2::Normalize(Vector2(cosf(degree_) * 2.0f, sinf(degree_)));
	auto resultDir = Vector2::Normalize(dir + addDir);
	resultDir /= 2;
	resultDir.y /= 2;

	//position_.y += sinf(degree_);

	position_ += resultDir * speed_ * (deltaTime * 60.0f);

	if (scale_.x > 0.0f) return;
	changeState(State::Stop);
	auto se = ResourceLoader::GetInstance().getSoundID(SoundID::SE_GETCOIN);
	ChangeNextPlayVolumeSoundMem(180, se);
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

void StarEffect::stop()
{
}

void StarEffect::setIsNotIdel()
{
	isIdel_ = false;
}

void StarEffect::changeMove()
{
	changeState(State::PositionMove);
}

void StarEffect::changeImbide()
{
	changeState(State::Imbide);
}

bool StarEffect::isIdelTimeEnd(const float time)
{
	return !isStart_ && state_ == State::Idel && stateTimer_ >= time;
}

bool StarEffect::isRotateMove()
{
	return state_ == State::RotateMove;
}

bool StarEffect::isRotateTimeEnd(const float time)
{
	return isRotateMove() && stateTimer_ >= time;
}

bool StarEffect::isImbide()
{
	return state_ == State::Imbide;
}

bool StarEffect::isStop()
{
	return state_ == State::Stop;
}
