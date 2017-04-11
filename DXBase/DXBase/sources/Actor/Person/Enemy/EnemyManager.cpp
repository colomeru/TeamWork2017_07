#include "EnemyManager.h"
#include "FloorSearchPoint.h"
#include "PlayerSearchObj.h"
#include "../../Base/ActorGroup.h"
#include "../../../World/IWorld.h"
#include "../../../Math/Math.h"

EnemyManager::EnemyManager(
	IWorld * world, 
	const Vector2 position, 
	const Vector2& direction) :
	distance_(1),
	wspResult_(0),
	wCollideCount_(0),
	wCollidePastCount_(wCollideCount_),
	timer_(0.0f),
	deltaTimer_(0.0f),
	boxMoveCount(0.0f),
	isDirection_(true),
	enemyPosition_(position),
	threadPosition_(position + Vector2::Left * 100.0f),
	enemyDirection_(Vector2::Zero),
	wsDirection_(Vector2(-1.0f, 0.0f)),
	animaDirection_(wsDirection_),
	world_(world)
{
	animaDirectionMap_.clear();
	wspDirectionMap_.clear();
	wallMoveConteiner_.push_back(0.0f);
	wallMoveConteiner_.push_back(1.0f);
	wallMoveConteiner_.push_back(0.0f);
	wallMoveConteiner_.push_back(-1.0f);
	// 素数を追加(3～4桁)
	primeContainer_.push_back(200);
	primeContainer_.push_back(300);
	primeContainer_.push_back(500);
	primeContainer_.push_back(700);
	primeContainer_.push_back(1100);
	primeContainer_.push_back(1300);
	primeContainer_.push_back(1700);
	primeContainer_.push_back(1900);
	// 壁移動の方向をマップに追加
	addWSPDirection(direction);
	// 位置コンテナの初期化
	positiones_.clear();
	psObjs.clear();
}

EnemyManager::~EnemyManager(){}

// 更新
void EnemyManager::update(float deltaTime)
{
	deltaTimer_ = deltaTime;
	timer_ += deltaTimer_;
}

// 壁に沿った動き方
Vector2 EnemyManager::wallMove()
{
	// 敵の四方に当たり判定のあるオブジェクトを配置
	// 当たったオブジェクトの名前などを参照して、方向の値を決定する
	return wsDirection_;
}

// 崖を避ける動き方
Vector2 EnemyManager::cliffMove(bool isFloor)
{
	auto posi = enemyPosition_;
	posi = posi.Left * (float)distance_;
	// 敵の前方下部に当たり判定のあるオブジェクトを配置
	// 向いている方向によって、オブジェクトの位置を変える
	return posi;
}

// プレイヤーの位置を取得します
Vector2 EnemyManager::getPlayerPosition(const int number)
{
	return positiones_[number];
}

// 指定したオブジェクトとの距離を返します
float EnemyManager::getLength(const Vector2 & otherPosition)
{
	auto length = otherPosition - enemyPosition_;
	return length.Length();
}

// プレイヤーとの距離を返します
float EnemyManager::getPlayerLength(const int number)
{
	// プレイヤーの位置を取得
	return getLength(positiones_[number]);
}

// 敵からプレイヤーに伸ばしたベクトルを返します
Vector2 EnemyManager::getPlayerVector(const int number)
{
	auto vector = positiones_[number] - enemyPosition_;
	return Vector2(std::abs(vector.x), std::abs(vector.y));
}

// 指定したオブジェクトとの方向を単位ベクトルで取得します
Vector2 EnemyManager::getDirection(const Vector2& otherPosition)
{
	// 方向の計算
	auto distance = enemyPosition_ - otherPosition;
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
		direction.y = 1;
	else if (distance.y > 0)
		direction.y = -1;
	else direction.y = 0;
	return direction;
}

// プレイヤーとの方向を単位ベクトルで取得します
Vector2 EnemyManager::getPlayerDirection(const int number)
{
	return getDirection(positiones_[number]);
}

// 指定したオブジェクトとの方向を、正規化されたベクトルで取得します
Vector2 EnemyManager::getNormalizeDirection(const Vector2 & otherPosition)
{
	// 方向の計算
	auto distance = otherPosition - enemyPosition_;
	return Vector2::Normalize(distance);
}

// プレイヤーとの方向を正規化されたベクトルで取得します
Vector2 EnemyManager::getPlayerNormalizeDirection(const int number)
{
	// 方向の計算
	return getNormalizeDirection(positiones_[number]);
}

//　敵自身とプレイヤーの位置を入れます
void EnemyManager::setEMPosition(
	const Vector2& enemyPosition,  
	const Vector2& direction)
{
	enemyPosition_ = enemyPosition;
	enemyDirection_ = direction;
}

// 糸の支点の位置取得
Vector2 EnemyManager::getThreadPoint()
{
	return threadPosition_;
}

// 捜索オブジェクトの設定
void EnemyManager::addFSP(FloorSearchPoint * fsp)
{
	wspContainer_.push_back(fsp);
}

// 壁に沿った動き方
Vector2 EnemyManager::getWallDirection()
{
	eachWSPObj();
	// 全部の値に割り振られていれば不要
	for (auto i = wspDirectionMap_.begin(); i != wspDirectionMap_.end(); i++) {
		// 方向を決められる場合か、ヒット数が1の場合　かつ
		// マップに入れてある値と同一ならば、方向を決める
		if (isDirecion() && i->first == wspResult_) {
			animaDirection_ = animaDirectionMap_[wspResult_];
			wsDirection_ = wspDirectionMap_[wspResult_];
			// 方向を決めたらfalseにする
			isDirection_ = false;
			break;
		}
	}
	// 過去のカウントを更新
	wCollidePastCount_ = wCollideCount_;
	return wsDirection_;
}

// 壁移動時のアニメーションの方向を取得します
Vector2 EnemyManager::getWallAnimaDirection()
{
	return animaDirection_;
}

void EnemyManager::setIsDirection(bool isDirection)
{
	isDirection_ = isDirection;
}

int EnemyManager::eachWSPObj()
{
	wspResult_ = 0;
	wCollideCount_ = 0;
	for (int i = 0; i != wspContainer_.size(); i++) {
		// 当たっているなら、コンテナの値とヒット数を加算する
		if (wspContainer_[i]->isGround()) {
			wspResult_ += primeContainer_[i];
			wspResult_ += 1;
			wCollideCount_ += 1;
		}
	}
	// 進行方向によって、値を加算する(2桁)
	if (wsDirection_.x != 0)
		wspResult_ += 10;
	else if (wsDirection_.y != 0)
		wspResult_ += 20;

	return wspResult_;
}

FloorSearchPoint* EnemyManager::getWSPObj(const int number)
{
	return wspContainer_[number];
}

bool EnemyManager::isDirecion()
{
	// カウントが同一なら、falseを返す
	if (wCollidePastCount_ == wCollideCount_) 
		return false;
	return true;
}

// 壁移動の方向を追加します
void EnemyManager::addWSPDirection(const Vector2 & direction)
{
	// mapに追加
	// 1つのみ(四隅) 2 5 13 19
	// X に移動量がある場合
	wspDirectionMap_[211] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[511] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[1311] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[1911] = Vector2(0.0f, 1.0f);
	// Y に移動量がある場合
	wspDirectionMap_[221] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[521] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[1321] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[1921] = Vector2(1.0f, 0.0f);
	// 1つのみ(上下左右) 3 7 11 17
	wspDirectionMap_[311] = Vector2(direction.x, 0.0f);
	wspDirectionMap_[721] = Vector2(0.0f, -direction.y);
	wspDirectionMap_[1121] = Vector2(0.0f, direction.y);
	wspDirectionMap_[1711] = Vector2(-direction.x, 0.0f);
	// 2つ当たる
	/*wspDirectionMap_[812] = Vector2(wsDirection_.x, 0.0f);
	wspDirectionMap_[822] = Vector2(wsDirection_.x, 0.0f);*/
	/*wspDirectionMap_[812] = Vector2(direction.x, 0.0f);
	wspDirectionMap_[822] = Vector2(direction.x, 0.0f);*/
	// 3つ当たる(辺) 10 22 35 49
	// Xに移動量がある
	wspDirectionMap_[1013] = Vector2(-direction.x, 0.0f);
	wspDirectionMap_[2213] = Vector2(0.0f, direction.y);
	wspDirectionMap_[3513] = Vector2(0.0f, -direction.y);
	wspDirectionMap_[4913] = Vector2(direction.x, 0.0f);
	// Yに移動量がある
	wspDirectionMap_[1023] = Vector2(-direction.x, 0.0f);
	wspDirectionMap_[2223] = Vector2(0.0f, direction.y);
	wspDirectionMap_[3523] = Vector2(0.0f, -direction.y);
	wspDirectionMap_[4923] = Vector2(direction.x, 0.0f);
	// 3つ当たる(隅の近辺) 12 19 27 47
	// X に移動量がある場合
	wspDirectionMap_[1213] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[1913] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[2713] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[4713] = Vector2(0.0f, -1.0f);
	// Y に移動量がある場合
	wspDirectionMap_[1223] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[1923] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[2723] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[4723] = Vector2(-1.0f, 0.0f);
	// 3つの(四隅の組み合わせ)当たる 20 28 34 37
	// X に移動量がある場合
	wspDirectionMap_[2013] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[2813] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[3413] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[3713] = Vector2(0.0f, -1.0f);
	// Y に移動量がある場合
	wspDirectionMap_[2023] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[2823] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[3423] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[3723] = Vector2(-1.0f, 0.0f);
	// 4つ当たる(隅) (0135) (0356) (2147) (2467) 
	// X に移動量がある場合
	wspDirectionMap_[2514] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[3814] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[3914] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[5214] = Vector2(0.0f, -1.0f);
	// Y に移動量がある場合
	wspDirectionMap_[2524] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[3824] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[3924] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[5224] = Vector2(-1.0f, 0.0f);
	// 5点当たる(隅) 30 40 58 65
	// X に移動量がある場合
	wspDirectionMap_[3015] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[4015] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[5815] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[6515] = Vector2(0.0f, -1.0f);
	//wspDirectionMap_[5515] = Vector2(0.0f, -1.0f);
	/*wspDirectionMap_[5515] = Vector2(0.0f, direction.y);*/
	// Y に移動量がある場合
	wspDirectionMap_[3025] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[4025] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[5825] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[6525] = Vector2(-1.0f, 0.0f);
	// 1つのみ当たらない 74 70 66 60
	wspDirectionMap_[7417] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[7017] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[6617] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[6017] = Vector2(0.0f, 1.0f);
	wspDirectionMap_[7427] = Vector2(0.0f, -1.0f);
	wspDirectionMap_[7027] = Vector2(-1.0f, 0.0f);
	wspDirectionMap_[6627] = Vector2(1.0f, 0.0f);
	wspDirectionMap_[6027] = Vector2(0.0f, 1.0f);
	// 全部当たっている
	//wspDirectionMap_[7718] = Vector2::Zero;
	//wspDirectionMap_[7728] = Vector2::Zero;

	// アニメーションの方向
	animaDirectionMap_[211] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[511] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[1311] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[1911] = Vector2(0.0f, -1.0f);
	// Y に移動量がある場合
	animaDirectionMap_[221] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[521] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[1321] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[1921] = Vector2(1.0f, 0.0f);
	// 1つのみ(上下左右) 3 7 11 17
	animaDirectionMap_[311] = Vector2(direction.x, 0.0f);
	animaDirectionMap_[721] = Vector2(0.0f, -direction.y);
	animaDirectionMap_[1121] = Vector2(0.0f, direction.y);
	animaDirectionMap_[1711] = Vector2(-direction.x, 0.0f);
	// 3つ当たる(辺) 10 22 35 49
	// Xに移動量がある
	animaDirectionMap_[1013] = Vector2(direction.x, 0.0f);
	animaDirectionMap_[2213] = Vector2(0.0f, direction.y);
	animaDirectionMap_[3513] = Vector2(0.0f, direction.y);
	animaDirectionMap_[4913] = Vector2(direction.x, 0.0f);
	// Yに移動量がある
	animaDirectionMap_[1023] = Vector2(direction.x, 0.0f);
	animaDirectionMap_[2223] = Vector2(0.0f, direction.y);
	animaDirectionMap_[3523] = Vector2(0.0f, direction.y);
	animaDirectionMap_[4923] = Vector2(direction.x, 0.0f);
	// 3つ当たる(隅の近辺) 12 19 27 47
	// X に移動量がある場合
	animaDirectionMap_[1213] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[1913] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[2713] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[4713] = Vector2(0.0f, 1.0f);
	// Y に移動量がある場合
	animaDirectionMap_[1223] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[1923] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[2723] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[4723] = Vector2(-1.0f, 0.0f);
	// 3つの(四隅の組み合わせ)当たる 20 28 34 37
	// X に移動量がある場合
	animaDirectionMap_[2013] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[2813] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[3413] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[3713] = Vector2(0.0f, 1.0f);
	// Y に移動量がある場合
	animaDirectionMap_[2023] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[2823] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[3423] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[3723] = Vector2(-1.0f, 0.0f);
	// 4つ当たる(隅) (0135) (0356) (2147) (2467) 
	// X に移動量がある場合
	animaDirectionMap_[2514] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[3814] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[3914] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[5214] = Vector2(0.0f, 1.0f);
	// Y に移動量がある場合
	animaDirectionMap_[2524] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[3824] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[3924] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[5224] = Vector2(-1.0f, 0.0f);
	// 5点当たる(隅) 30 40 58 65
	// X に移動量がある場合
	animaDirectionMap_[3015] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[4015] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[5815] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[6515] = Vector2(0.0f, 1.0f);
	// Y に移動量がある場合
	animaDirectionMap_[3025] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[4025] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[5825] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[6525] = Vector2(-1.0f, 0.0f);
	// 1つのみ当たらない 74 70 66 60
	animaDirectionMap_[7417] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[7017] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[6617] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[6017] = Vector2(0.0f, 1.0f);
	animaDirectionMap_[7427] = Vector2(0.0f, -1.0f);
	animaDirectionMap_[7027] = Vector2(-1.0f, 0.0f);
	animaDirectionMap_[6627] = Vector2(1.0f, 0.0f);
	animaDirectionMap_[6027] = Vector2(0.0f, 1.0f);
}

// プレイヤーの位置を設定します
void EnemyManager::setPlayerPosition(const int number, const Vector2& position)
{
	positiones_[number] = position;
	psObjs[number]->setPosition(enemyPosition_, position);
}

// プレイヤー捜索オブジェクトの追加を行います
void EnemyManager::setPSObj(const int number, const Vector2 & position)
{
	auto psObj = std::make_shared<PlayerSearchObj>(
		world_, position, positiones_[number]);
	world_->addActor(ActorGroup::EnemyBullet, psObj);
	psObjs[number] = psObj.get();
}

// プレイヤー捜索オブジェクトの取得
PlayerSearchObj * EnemyManager::getPSObj(const int number)
{
	return psObjs[number];
}

// 所持しているオブジェクトの削除を行います
void EnemyManager::deleteObj()
{
	for (auto i = psObjs.begin(); i != psObjs.end(); i++) {
		auto a = *i;
		a.second->dead();
		psObjs[i->first] = nullptr;
	}
	psObjs.clear();
}
