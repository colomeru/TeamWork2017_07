#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "../../../Math/Math.h"
#include <vector>
#include <map>

class FloorSearchPoint;
class PlayerSearchObj;
class IWorld;

class EnemyManager{
public:
	EnemyManager(
		IWorld* world,
		const Vector2 position, 
		const Vector2& direction = Vector2(-1.0f, -1.0f));
	~EnemyManager();
	// 更新
	void update(float deltaTime);
	// 壁に沿った動き方（まだ）
	Vector2 wallMove();
	// 崖を避ける動き方
	Vector2 cliffMove(bool isFloor);
	// プレイヤーの位置を取得します
	Vector2 getPlayerPosition(const int number);
	// 指定したオブジェクトとの距離を返します
	float getLength(const Vector2& otherPosition);
	// プレイヤーとの距離を返します
	float getPlayerLength(const int number);
	// 敵からプレイヤーに伸ばしたベクトルを返します
	Vector2 getPlayerVector(const int number);
	// 指定したオブジェクトとの方向を単位ベクトルで取得します
	Vector2 getDirection(const Vector2& otherPosition);
	// プレイヤーとの方向を単位ベクトルで取得します
	Vector2 getPlayerDirection(const int number);
	// 指定したオブジェクトとの方向を、正規化されたベクトルで取得します
	Vector2 getNormalizeDirection(const Vector2& otherPosition);
	// プレイヤーとの方向を正規化されたベクトルで取得します
	Vector2 getPlayerNormalizeDirection(const int number);
	//　敵自身とプレイヤーの位置を入れます
	void setEMPosition(
		const Vector2& enemyPosition,
		const Vector2& direction);
	// 糸の支点の位置取得
	Vector2 getThreadPoint();
	// 捜索オブジェクトの設定
	void addFSP(FloorSearchPoint* fsp);
	// 壁に沿った動き方
	Vector2 getWallDirection();
	// 壁移動時のアニメーションの方向を取得します
	Vector2 getWallAnimaDirection();
	// 方向を決めるか
	void setIsDirection(bool isDirection);
	// 壁オブジェクトを回して、合計値を返します
	int eachWSPObj();
	// 指定した要素数の壁捜索オブジェクトを取得します
	FloorSearchPoint* getWSPObj(const int number);
	// mapに追加処理を行います
	void initWSPMap();
	// 壁移動の方向を決定するかを返します
	bool isDirecion();
	// 壁移動の方向を追加します
	void addWSPDirection(const Vector2& direction);
	// プレイヤーの位置を設定します
	void setPlayerPosition(const int number, const Vector2& position);
	// プレイヤー捜索オブジェクトの追加を行います
	void setPSObj(const int number, const Vector2 & position);
	// プレイヤー捜索オブジェクトの追加を行います
	PlayerSearchObj* getPSObj(const int number);
	// 所持しているオブジェクトの削除を行います
	void deleteObj();

private:
	int distance_;				// 方向
	int wspResult_;				// 壁移動に使う値の合計値
	int wCollideCount_;			// 壁に当たっているオブジェクトの数
	int wCollidePastCount_;		// 壁に当たっているオブジェクトの過去の数
	float timer_;				// 経過時間
	float deltaTimer_;			// 時間
	float boxMoveCount;			// 四角形移動カウント
	bool isDirection_;			// 方向を代入するか
	Vector2 enemyPosition_;		// 敵の位置
	Vector2 threadPosition_;	// 糸の位置
	Vector2 enemyDirection_;	// エネミーの方向
	Vector2 wsDirection_;		// 壁移動時の方向
	Vector2 animaDirection_;	// 壁移動時のアニメーションの方向
	IWorld* world_;				// ワールド
	// プレイヤー判別用コンテナ
	typedef std::map<int, Vector2> PlayerPositionMap;
	PlayerPositionMap positiones_;
	// プレイヤー捜索オブジェクトコンテナ
	typedef std::map<int, PlayerSearchObj*> PSObjMap;
	PSObjMap psObjs;
	// 四角形移動用コンテナ
	typedef std::vector<float> WallMoveConteiner;
	WallMoveConteiner wallMoveConteiner_;
	// 壁捜索オブジェクトコンテナ
	typedef std::vector<FloorSearchPoint*> WSPContainer;
	WSPContainer wspContainer_;
	// 素数コンテナ
	typedef std::vector<int> PrimeContainer;
	PrimeContainer primeContainer_;
	// アニメーション方向コンテナ
	// <int, [Vector2, Vector2]>
	typedef std::map<int, Vector2> animaDirectionMap;
	animaDirectionMap animaDirectionMap_;
	// 壁移動の方向コンテナ
	typedef std::map<int, Vector2> WSPDirectionMap;
	WSPDirectionMap wspDirectionMap_;
};

#endif