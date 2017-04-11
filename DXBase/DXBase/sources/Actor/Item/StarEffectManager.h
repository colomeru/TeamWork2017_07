#ifndef STAR_EFFECT_MANAGER_H_
#define STAR_EFFECT_MANAGER_H_

//#include "DxLib.h"
#include "../../Math/Math.h"
#include <vector>
#include <list>

class IWorld;
class StarEffect;

class StarEffectManager {
public:
	StarEffectManager();
	StarEffectManager(IWorld* world);
	~StarEffectManager() {}
	void initialeze();
	void update();
	void end();

public:
	// 星の位置の設定
	void setStartPosition(const Vector2& position, const float addPositionX);
	// 星の演出が終了したかを返します
	bool isEffectEnd();

private:
	// 星の生成
	void createStars();
	// 星の更新
	void updateStars();
	// 星を動かします
	void moveStars();

private:
	// 星関連
	int starCount_;						// 星の生成数
	bool isCreateStar_;					// 星を生成したか
	bool isIdelEnd_;					// 待機状態が終了しているか
	bool isPowerUpCreate_;				// エフェクトを生成したか
	bool isEffectEnd_;					// 演出が終了したか
	IWorld* world_;						// ワールド
	// 星の生成位置コンテナ
	std::vector<Vector2> startPositiones_;
	// 星格納コンテナ
	typedef std::list<StarEffect*> StarContainer;
	StarContainer stars_;
};


#endif