#ifndef BOSS_ATTACK_H_
#define BOSS_ATTACK_H_

#include "../../../../../Math/Math.h"
#include "../../../../../Define.h"
#include "../BossAnimationNumber.h"
#include "../../../../../World/IWorld.h"
#include "./../../../../Base/ActorGroup.h"
#include <string>
#include <random>	// C++11の機能

// ボス攻撃クラス(ベース)
class BossAttack {
public:
	BossAttack();
	BossAttack(IWorld* world, const Vector2& position);
	// デストラクタ
	virtual ~BossAttack();
	// 更新
	void update(float deltaTime);
	// 攻撃
	virtual void attack(float deltaTime);
	// 移動した位置を取得します
	Vector2 getMovePosition();
	// 方向を取得します
	Vector2 getDirection();
	// 攻撃行動のリフレッシュを行います
	virtual void Refresh();
	// 攻撃が開始したかを返します
	bool isAttackStart();
	// 攻撃が終了したかを返します
	bool isAttackEnd();

public:
	// ボスの心臓の体力を設定します
	void setHeartHP(const int hp);
	// 位置を設定します
	void setPosition(const Vector2& position);
	// プレイヤーの位置を設定します
	void setPlayerPosition(const Vector2& position);
	// プレイヤーとの方向を設定します
	void setPlayerDirection(const Vector2& direction);
	// プレイヤーとの正規化された方向を設定します
	void setPlayerNormalizeDirection(const Vector2& direction);
	// 壁捜索オブジェクトが当たったかを設定します
	void setIsWallHit(bool isHit);
	// 壁移動の方向を取得します
	Vector2 getMoveDirection();
	// 床に触れているかを設定します
	void setIsGround(bool isGround);
	// 壁の下側に当たったかを設定します
	void setIsBottom(bool isBottom);
	// 触れている床の名前を設定します
	void setFloorName(const char* name);
	// 攻撃可能状態かを設定します
	void setIsMove(bool isMove);
	// ひるみカウントを返します
	int getFlinchCount();
	// 角度を返します
	float getAngle();
	// 怯むかを返します
	bool isFlinch();
	// 重力を使用するかを返します
	bool isUseGravity();
	// プレイヤーの攻撃に当たるかを返します
	bool isBodyHit();
	// プレイヤーに当たるかを返します
	bool isAttackHit();
	// 衝突したオブジェクトを設定します
	void setCollideObj(Actor& actor);
	// アニメーション番号を返します
	int getAnimaNum();
	// アニメーションの角度を返します
	int getAnimeAngle();
	// アニメーションがループするかを返します
	bool isLoop();
	// アニメーションを逆再生するかを返します
	bool isReverse();

protected:
	// 整数型のランダム数を返します
	int getRandomInt(int min, int max);

protected:
	int hp_;					// 体力
	int flinchCount_;			// ひるむまでの回数
	float timer_;				// 時間
	float angle_;				// 角度
	float animeAngle_;			// アニメーションの角度
	std::string floorName_;		// 床の名前
	bool isAttackStart_;		// 攻撃を開始したか
	bool isAttackEnd_;			// 攻撃が終了したか
	bool isGround_;				// ボスが床に触れているのか
	bool isBottom_;				// 壁の下側に当たったか
	bool isUseGravity_;			// 重力を使うか
	bool isWspHit_;				// 壁捜索オブジェクトが当たったか
	bool isPrevWspHit_;			// 過去に壁捜索オブジェクトが当たったか
	bool isBodyHit_;			// プレイヤー本体に当たるか
	bool isAttackHit_;			// プレイヤーの攻撃に当たるか
	bool isMove_;				// 動ける状態か
	bool isFlinch_;				// 怯むか
	bool isAnimaLoop_;			// アニメーションをループさせるか
	bool isAnimaReverse_;		// アニメーションを逆再生するか
	Vector2 position_;			// 位置
	Vector2 direction_;			// 方向
	Vector2 wsDirection_;		// 壁捜索オブジェクトの方向
	Vector2 pPosition_;			// プレイヤーの位置
	Vector2 pDirection_;		// プレイヤーとの方向
	Vector2 pNormDirection_;	// プレイヤーとの方向
	IWorld* world_;				// ワールドポインタ
	int animeNum_;				// アニメーション番号
	Actor* collideObj_;			// 衝突したオブジェクト
	std::mt19937 mt_;			// 乱数の初期seed
};

#endif
