#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "../../Base/Actor.h"
#include "EnemyManager.h"
#include "../../../Animation/Enemy/EnemyAnimation2D.h"
#include "../../../ResourceLoader/ResourceLoader.h"
#include <vector>

class CollisionBase;
class FloorSearchPoint;
class Prickle;

class BaseEnemy : public Actor {
protected:
	// アニメーションID
	enum {
		ENEMY_WALK = 0,
		ENEMY_WALKTURN = 4,
		ENEMY_DISCOVERY = 3,
		ENEMY_ATTACK = 1,
		ENEMY_ATTACKTURN = 5,
		ENEMY_DAMAGE = 2,
	};
	//// エフェクトID
	//enum {
	//	EFFECT_DEAD = 0
	//};
	// SEID
	enum {
		SE_HAKKEN = 0,
		SE_HITSTOP = 1,
		SE_DEAD = 2
	};
	// プレイヤー番号
	enum {
		PLAYER_RED_NUMBER = 0,
		PLAYER_BLUE_NUMBER = 1
	};
	// 状態列挙
	enum class State {
		Idel,
		Search,
		Discovery,
		Chase,
		Lost,
		attack,
		Damage,
		Dead,
		Return
	};

public:
	// 矩形
	BaseEnemy(
		IWorld* world,
		const Vector2&  position,
		const float bodyScale,
		const Vector2& direction = Vector2(1.0f, 1.0f));
	~BaseEnemy();
	void Initialize();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& actor) override;
	virtual void onMessage(EventMessage event, void*) override;

protected:
	// 子供用のupdate(親のupdate前に行います)
	virtual void beginUpdate(float deltaTime);
	// 子供用のupdate
	virtual void update(float deltaTime);
	// 待機状態です
	void idle();
	// 索敵移動です
	virtual void search();
	// プレイヤーを発見した時の行動です
	virtual void discovery();
	// プレイヤーの追跡行動です
	virtual void chase();
	// 攻撃行動です
	virtual void attack();
	// 被弾行動です
	void damageMove();
	// 死亡行動です
	void deadMove();
	// プレイヤーを見失ったときの行動です
	virtual void lostMove();
	// 状態の変更を行います
	void changeState(State state, unsigned int motion);
	// 所持しているオブジェクトの位置を設定します
	virtual void setObjPosition();
	// プレイヤーを捜索します
	void findPlayer();

protected:
	// 索敵時の行動です
	virtual void searchMove();
	// 追跡時の行動です
	virtual void chaseMove();
	// 床捜索オブジェクトの生成
	void createFSP();
	// デルタタイム(最大値1)の設定
	void setDeltaTime(float deltatime);

public:
	// 敵が飲み込まれた時のスケールポイントを返します
	float getSP();
	// 敵の大きさを返します
	int getScale();

protected:
	// 状態の更新を行います
	void updateState(float deltaTime);
	// 捜索オブジェクト関連の更新
	void updateSearchObjct();
	// 衝突関連の更新
	void updateCollide();
	// 地面の位置に補正します
	void groundClamp(Actor& actor);
	// 円と衝突したときに位置に補正します
	void circleClamp(Actor& actor);
	// アニメーションの追加を行います
	virtual void addAnimation();
	// プレイヤーとのX方向とY方向を計算し、画面外にいるかを返します
	bool isScreen();
	// ポーズ中にSEの停止を行います
	void poseStopSE();
	// ポーズ解除時にSEを再度再生します
	void poseRestartSE();

protected:
	// メンバ変数
	float timer_;					// 現在の時間(補間)
	float deltaTimer_;				// 現在の時間(補間, 最大値 1)
	int hp_;						// 体力
	int ap_;						// アタックポイント
	int texSize_;					// テクスチャを切り抜きする大きさ
	int turnMotion_;
	int discoveryNumber_;			// 発見番号
	float speed_;					// 移動速度
	float initSpeed_;				// 初期の移動速度
	float scale_;					// 大きさ
	float playerLength_;			// プレイヤーとの距離
	float discoveryLenght_;			// プレイヤーに気づく距離
	float playerLostLenght_;		// プレイヤーを見失う距離
	float texDegress_;				// テクスチャの角度
	float hitTimer_;				// 衝突時の時間
	Vector2 direction_;				// 方向
	Vector2 prevDirection_;			// 過去の方向
	bool isPlayer_;					// プレイヤーが存在するのか
	bool isMove_;					// 動くか
	bool isScreen_;					// 画面内にいるか
	bool isBlockCollideBegin_;		// ブロックと当たっているか(初回時)
	bool isBlockCollideEnter_;		// ブロックと当たっているか(衝突中)
	bool isBlockCollidePrevEnter_;	// ブロックと当たっているか(過去の衝突中)
	bool isBlockCollideExit_;		// ブロックと当たっているか(衝突後)
	bool isGround_;					// 接地しているか trueで接地しています
	bool isUseGravity_;				// 重力を使うか trueで重力を追加します
	bool isInvincible_;				// 無敵か trueで無敵になります
	bool isLostChase_;				// 見失っても追うか trueでプレイヤーが範囲外に出ても、追跡行動を行います
	bool isMoveFloor_;				// 動いている床に触れているか
	float stateTimer_;				// 状態タイマ
	State state_;					// 状態
	Vector2 discoveryPosition_;		// 発見したときの位置
	Vector2 addTexPosition_;		// テクスチャの表示位置の追加
	EnemyManager enemyManager_;		// エネミーマネージャー
	FloorSearchPoint* fspScript_;	// 床捜索オブジェクト
	FloorSearchPoint* wsScript_;		// 壁捜索オブジェクト
	Prickle* pricleObj_;			// トゲのオブジェクト
	EnemyAnimation2D animation_;	// アニメーション
	AnimationID deadAnimaID_;		// 死亡時のアニメーション
	// 捜索オブジェクトの位置コンテナ
	typedef std::vector<Vector2> FSPPositionContainer;
	FSPPositionContainer fspPositionContainer_;
	// 捜索オブジェクトの大きさコンテナ
	typedef std::vector<Vector2> FSPScaleContainer;
	FSPScaleContainer fspScaleContainer_;
	// 所持しているオブジェクトのコンテナ
	typedef std::list<Actor*> ObjContainer;
	ObjContainer objContainer_;
	// SEハンドルコンテナ
	typedef std::vector<int> SEContainer;
	SEContainer seHandles_;
	// 再生SEハンドルコンテナ
	typedef std::list<int> PlaySEContainer;
	PlaySEContainer playSEHandles_;
	// 重力加速度
	const float GRAVITY_ = 9.8f;
};

#endif