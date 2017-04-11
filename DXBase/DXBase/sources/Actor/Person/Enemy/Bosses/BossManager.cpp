#include "BossManager.h"
#include "bossAttack/BossAttack.h"
#include "../../../../World/IWorld.h"
#include "../../../../Math/Math.h"

BossManager::BossManager() : 
	attackNumber_(0),
	bossPosition_(Vector2::Zero),
	playerPosition_(Vector2::Zero)
{}

BossManager::BossManager(IWorld* world, const Vector2 & position) :
	attackNumber_(0), 
	bossPosition_(position),
	playerPosition_(Vector2::Zero)
{}

// デストラクタ
BossManager::~BossManager()
{
	bossAttackContainer_.clear();
}

// 攻撃コンテナに攻撃を追加します
void BossManager::addAttack(std::shared_ptr<BossAttack> attack)
{
	bossAttackContainer_.push_back(attack);
}

// 攻撃の番号を変更します
void BossManager::changeAttackNumber(const int number)
{
	// 番号の変更
	attackNumber_ = number;
}

// 指定した番号の攻撃行動を行います
void BossManager::attackMove(const float deltaTime)
{
	// 攻撃
	bossAttackContainer_[attackNumber_]->update(deltaTime);
}

// 行動のリフレッシュを行います
void BossManager::attackRefresh()
{
	bossAttackContainer_[attackNumber_]->Refresh();
}

// 行動によって移動した位置を返します
Vector2 BossManager::getMovePosition()
{
	return bossAttackContainer_[attackNumber_]->getMovePosition();
}

// 攻撃方向を取得します
Vector2 BossManager::getAttackDirection()
{
	return bossAttackContainer_[attackNumber_]->getDirection();
}

// 攻撃が開始したかを返します
bool BossManager::isAttackStart()
{
	return bossAttackContainer_[attackNumber_]->isAttackStart();
}

// 攻撃が終了したかを返します
bool BossManager::isAttackEnd()
{
	return bossAttackContainer_[attackNumber_]->isAttackEnd();
}

// プレイヤーの攻撃に当たるかを返します
bool BossManager::IsBodyHit()
{
	return bossAttackContainer_[attackNumber_]->isBodyHit();
}

// プレイヤーに当たるかを返します
bool BossManager::IsAttackHit()
{
	return bossAttackContainer_[attackNumber_]->isAttackHit();
}

// 怯むかを返します
bool BossManager::isFlinch()
{
	return bossAttackContainer_[attackNumber_]->isFlinch();
}

// ひるみカウントを返します
int BossManager::getFlinchCount()
{
	return bossAttackContainer_[attackNumber_]->getFlinchCount();
}

// 角度を返します
float BossManager::getAngle()
{
	return bossAttackContainer_[attackNumber_]->getAngle();
}

// ボスの心臓の体力を設定します
void BossManager::setHeartHP(const int hp)
{
	bossAttackContainer_[attackNumber_]->setHeartHP(hp);
}

// ボスの位置を設定します
void BossManager::setPosition(const Vector2& position)
{
	bossPosition_ = position;
}

// プレイヤーの位置を設定します
void BossManager::setPlayerPosition(const Vector2 & position)
{
	playerPosition_ = position;
	bossAttackContainer_[attackNumber_]->setPlayerPosition(position);
}

// 攻撃前の位置を決定します
void BossManager::prevPosition()
{
	bossAttackContainer_[attackNumber_]->setPosition(bossPosition_);
}

Vector2 BossManager::getDirection(const Vector2 & otherPosition)
{
	// 方向の計算(ボスの位置を主軸にする)
	auto distance = otherPosition - bossPosition_;
	auto direction = Vector2().Zero;
	// 方向の値を代入
	// X
	if (distance.x < 0)
		direction.x = -1;
	else if (distance.x > 0)
		direction.x = 1;
	else direction.x = 0;
	// Y
	if (distance.y < 0)
		direction.y = -1;
	else if (distance.y > 0)
		direction.y = 1;
	else direction.y = 0;

	return direction;
}

Vector2 BossManager::getPlayerDirection()
{
	// bossAttackに方向を設定する
	auto direction = getDirection(playerPosition_);
	bossAttackContainer_[attackNumber_]->setPlayerDirection(direction);
	return direction;
}

// 指定したオブジェクトとの方向を正規化ベクトルで取得します
Vector2 BossManager::getNormalizeDirection(const Vector2 & otherPosition)
{
	auto distance = otherPosition - bossPosition_;
	auto direction = distance.Normalize();
	return direction;
}

// プレイヤーとの方向を正規化ベクトルで取得します
Vector2 BossManager::getPlayerNormalizeDirection()
{
	auto direction = getNormalizeDirection(playerPosition_);
	bossAttackContainer_[attackNumber_]->setPlayerNormalizeDirection(direction);
	return direction;
}

void BossManager::setIsGround(bool isGround)
{
	bossAttackContainer_[attackNumber_]->setIsGround(isGround);
}

// ボスが壁の下側に当たったかを設定します
void BossManager::setIsBottom(bool isBottom)
{
	bossAttackContainer_[attackNumber_]->setIsBottom(isBottom);
}

// ボスが当たった床の名前を返します
void BossManager::setFloorName(const char * name)
{
	bossAttackContainer_[attackNumber_]->setFloorName(name);
}

// 攻撃中に重力を使用するかを返します
bool BossManager::isUseGravity()
{
	return bossAttackContainer_[attackNumber_]->isUseGravity();
}

// 攻撃が可能な状態かを設定します
void BossManager::setIsAttackMove(bool isMove)
{
	bossAttackContainer_[attackNumber_]->setIsMove(isMove);
}

// 壁捜索オブジェクトが当たったかを設定します
void BossManager::setIsWallHit(bool isHit)
{
	bossAttackContainer_[attackNumber_]->setIsWallHit(isHit);
}

Vector2 BossManager::getWallMoveDirection()
{
	return bossAttackContainer_[attackNumber_]->getMoveDirection();
}

// 衝突したオブジェクトを設定します
void BossManager::setCollideObj(Actor & actor)
{
	bossAttackContainer_[attackNumber_]->setCollideObj(actor);
}

// アニメーション番号を返します
int BossManager::getAnimaNum()
{
	return bossAttackContainer_[attackNumber_]->getAnimaNum();
}

// アニメーションの角度を返します
int BossManager::getAnimeAngle()
{
	return bossAttackContainer_[attackNumber_]->getAnimeAngle();
}

// アニメーションがループするかを返します
bool BossManager::isAnimeLoop()
{
	return bossAttackContainer_[attackNumber_]->isLoop();
}

// アニメーションが逆再生するかを返します
bool BossManager::isAnimeReverse()
{
	return bossAttackContainer_[attackNumber_]->isReverse();
}
