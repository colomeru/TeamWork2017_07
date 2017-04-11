#include "FlyingEnemy.h"
#include "../../../Base/ActorGroup.h"
#include "../EnemyHeaderImport.h"
#include "../FloorSearchPoint.h"
#include "Prickle.h"
#include "../PlayerSearchObj.h"

FlyingEnemy::FlyingEnemy(
	IWorld * world,
	const Vector2 & position, 
	float Down) :
	BaseEnemy(world, position, 64.0f),
	direTimer_(0.0f),
	lostTimer_(0.0f),
	pastPosition_(Vector2::Zero),
	wsObj_(nullptr)
{
	direction_ = Vector2(1.0f, Down);
	BaseEnemy::Initialize();

	isUseGravity_ = false;
	deadAnimaID_ = AnimationID::ENEMY_FLYINGENEMY_DAMAGE_TEX;
	// 壁捜索オブジェクトの生成
	auto wsObj = std::make_shared<FloorSearchPoint>(
		world_, position_,
		Vector2(0.0f, -(scale_ / 2.0f + 1.0f)), 
		Vector2(scale_, 2.0f));
	// ワールドに追加
	world_->addActor(ActorGroup::Enemy, wsObj);
	wsObj_ = &*wsObj;
	objContainer_.push_back(wsObj_);
	// トゲオブジェクトの生成
	auto pricleObj = std::make_shared<Prickle>(
		world_,
		Vector2::Zero,
		Vector2::Up * -(scale_ + 1.0f),
		Vector2(64.0f, 64.0f));
	world_->addActor(ActorGroup::Enemy_AttackRange, pricleObj);
	pricleObj_ = &*pricleObj;
	objContainer_.push_back(pricleObj_);
	pricleObj_->setDirection(direction_);
	// アニメーションの追加
	addTexPosition_ = Vector2::Zero;
}

void FlyingEnemy::beginUpdate(float deltaTime)
{
	// トゲの更新
	pricleObj_->setEnemyPosition(position_);
}

void FlyingEnemy::update(float deltaTime)
{
	wsObj_->setDirection(direction_);
	wsObj_->setPosition(position_);
}

void FlyingEnemy::onMessage(EventMessage event, void *){}

// 索敵移動です
void FlyingEnemy::search()
{
	// プレイヤーの捜索
	findPlayer();
	// 初期速度に戻す
	speed_ = initSpeed_;
	// 捜索行動
	searchMove();
	// プレイヤーが存在しなければ、捜索と待機状態以外は行わない
	if (!isPlayer_) return;
	// 一定距離内で、プレイヤーとの間にブロックがなかったら
	// 追跡する
	for (int i = 0; i != 1; i++) {
		auto a = enemyManager_.getPlayerNormalizeDirection(i);
		auto b = Vector2::Right * direction_;
		auto radius = std::atan2f(b.x * a.y - a.x * b.y, a.x * b.x + a.y * b.y);
		auto deg = MathHelper::ToDegrees(radius);
		// 追跡する
		// psObj_->isPlayerLook()
		if (enemyManager_.getPlayerLength(i) <= discoveryLenght_ &&
			std::abs(deg) <= 30.0f &&
			enemyManager_.getPSObj(i)->isPlayerLook()) {
			changeState(State::Chase, ENEMY_ATTACK);
			discoveryPosition_ = position_;
			// 過去の位置を入れる
			auto player = world_->findActor("PlayerBody1");
			pastPosition_ = player->getPosition();
			pricleObj_->setDirection(enemyManager_.getDirection(pastPosition_));
			// 回転
			if (enemyManager_.getDirection(pastPosition_).x < 0)
				texDegress_ = 90;
			else texDegress_ = 270;
			break;
		}
	}
	/*if (enemyManager_.getPlayerLength() <= discoveryLenght_ &&
		psObj_->isPlayerLook()) {
		
	}*/
}

void FlyingEnemy::discovery(){}

void FlyingEnemy::attack(){}

void FlyingEnemy::searchMove()
{
	// 速度 / チップサイズの大きさ
	// => direTimer_ = 1 => マップチップ一個分動いたことにする	
	direTimer_ += (speed_ / CHIPSIZE) * deltaTimer_;
	// 床に当たるか、一定のマップチップ分動いたら、方向転換
	if (wsObj_->isGround() || direTimer_ >= 3.0f) {
		direction_.y *= -1;
		direTimer_ = 0;
	}
	position_ += Vector2::Down * speed_ * direction_.y * deltaTimer_;
	pricleObj_->setDirection(Vector2::Up);
	pricleObj_->setAddPosition(Vector2::Up * -(scale_ + 1.0f));
}

// プレイヤーの追跡行動です
void FlyingEnemy::chase()
{
	// プレイヤーの捜索
	findPlayer();
	// 移動速度を倍速にする
	speed_ = initSpeed_ * 1.5f;
	// 追跡行動
	chaseMove();
}

void FlyingEnemy::chaseMove()
{
	// プレイヤーのいた方向に進む
	// プレイヤーが居た位置との距離が０ならば、見失う
	auto direction = enemyManager_.getDirection(pastPosition_);
	auto p_Direction = position_ - pastPosition_;
 	auto length = p_Direction.Length();
	// プレイヤーが床にいる場合、一定の距離内に居ることが難しいので、切り上げる
	if (length <= 15) {
		//changeState(State::Lost, ENEMY_LOST);
		changeState(State::Lost, ENEMY_WALK);
		texDegress_ = 0.0f;
		return;
	}
	// 接地していたら、yベクトルを 0 にする
	if (wsObj_->isGround() || isGround_)
		p_Direction.y = 0;
	auto speed = speed_;
	if (length < speed_)
		speed = length;
	// プレイヤーの居た位置からの方向を設定
	direction_ = enemyManager_.getNormalizeDirection(pastPosition_);
	// プレイヤーの居た位置に進む
	position_ += direction_ * speed * deltaTimer_;
	// トゲの設定
	pricleObj_->setAddPosition(Vector2::Left * (scale_ + 1.0f));
}

// 敵を見失った時の行動です
void FlyingEnemy::lostMove()
{
	// 見失う
	lostTimer_ += deltaTimer_ / 60.0f;
	if (lostTimer_ <= 8.0f) return;
	lostTimer_ = 0.0f;
	changeState(State::Search, ENEMY_WALK);
}

// アニメーションの追加を行います
void FlyingEnemy::addAnimation()
{
	animation_.addAnimation(
		ENEMY_WALK,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::ENEMY_FLYINGENEMY_WALK_TEX));
	animation_.addAnimation(
		ENEMY_ATTACK,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::ENEMY_FLYINGENEMY_ATTACK_TEX));
	animation_.addAnimation(
		ENEMY_DAMAGE,
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::ENEMY_FLYINGENEMY_DAMAGE_TEX));
}
