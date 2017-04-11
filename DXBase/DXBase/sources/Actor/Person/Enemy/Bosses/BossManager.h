#ifndef BOSS_MANAGER_H_
#define BOSS_MANAGER_H_

#include "bossAttack/BossAttack.h"
#include <vector>
#include <memory>

class IWorld;

// ボスマネージャー 
class BossManager {
public:
	BossManager();
	// コンストラクタ
	BossManager(IWorld* world, const Vector2& position);
	// デストラクタ
	~BossManager();
	// 攻撃コンテナに攻撃を追加します
	void addAttack(std::shared_ptr<BossAttack> attack);
	// 攻撃の番号を変更します
	void changeAttackNumber(const int number);
	// 指定した番号の攻撃行動を行います
	void attackMove(const float deltaTime);
	// 攻撃のリフレッシュを行います
	void attackRefresh();
	// 行動によって移動した位置を返します
	Vector2 getMovePosition();
	// 攻撃方向を取得します
	Vector2 getAttackDirection();
	// 攻撃が開始したかを返します
	bool isAttackStart();
	// 攻撃が終了したかを返します
	bool isAttackEnd();
	// プレイヤーの攻撃に当たるかを返します
	bool IsBodyHit();
	// プレイヤーに当たるかを返します
	bool IsAttackHit();
	// 怯むかを返します
	bool isFlinch();
	// ひるみカウントを返します
	int getFlinchCount();
	// 角度を返します
	float getAngle();

public:
	// ボスの心臓の体力を設定します
	void setHeartHP(const int hp);
	// ボスの位置を設定します
	void setPosition(const Vector2& position);
	// プレイヤーの位置を設定します
	void setPlayerPosition(const Vector2& position);
	// 攻撃前の位置を決定します
	void prevPosition();
	// 指定したオブジェクトとの方向を単位ベクトルで取得します
	Vector2 getDirection(const Vector2& otherPosition);
	// プレイヤーとの方向を単位ベクトルで取得します
	Vector2 getPlayerDirection();
	// 指定したオブジェクトとの方向を正規化ベクトルで取得します
	Vector2 getNormalizeDirection(const Vector2& otherPosition);
	// プレイヤーとの方向を正規化ベクトルで取得します
	Vector2 getPlayerNormalizeDirection();
	// ボスが接地しているかを設定します
	void setIsGround(bool isGround);
	// ボスが壁の下側に当たったかを設定します
	void setIsBottom(bool isBottom);
	// ボスが当たった床の名前を返します
	void setFloorName(const char* name);
	// 攻撃中に重力を使用するかを返します
	bool isUseGravity();
	// 攻撃が可能な状態かを設定します
	void setIsAttackMove(bool isMove);
	// 壁捜索オブジェクトが当たったかを設定します
	void setIsWallHit(bool isHit);
	// 壁の移動方向
	Vector2 getWallMoveDirection();
	// 衝突したオブジェクトを設定します
	void setCollideObj(Actor& actor);
	// アニメーション番号を返します
	//BossAnimationNumber getAnimaNum();
	int getAnimaNum();
	// アニメーションの角度を返します
	int getAnimeAngle();
	// アニメーションがループするかを返します
	bool isAnimeLoop();
	// アニメーションが逆再生するかを返します
	bool isAnimeReverse();

private:
	int attackNumber_;			// 攻撃番号
	Vector2 bossPosition_;		// ボスの位置
	Vector2 playerPosition_;	// プレイヤーの位置
	// ボス攻撃コンテナ
	typedef std::vector<std::shared_ptr<BossAttack>> BossAttackContainer;
	BossAttackContainer bossAttackContainer_;
};

#endif
