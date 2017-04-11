#ifndef ENMEY_ANIMATION2D_H_
#define ENMEY_ANIMATION2D_H_

#include "../Base/Animation2D.h"

#include <unordered_map>

class EnemyAnimation2D : public Animation2D {

public:
	EnemyAnimation2D();
	~EnemyAnimation2D();
	// 更新
	void update(float deltaTime) override;
	// アニメーションの追加
	// (アニメーションのID, リソースID, 横の数, 縦の数, 横の数の減らる数)
	void addAnimation(int id, const std::vector<int>& anims);
	// アニメーションの変更
	void changeAnimation(int id, float speed = 1.0f);
	// アニメーションのが終わったかを返します
	bool isEndAnimation();
	// 振り向きアニメーションを行って、画像を反転します
	void turnAnimation(int id, float direction);
	// アニメーションを振り向きアニメーションを行わずに画像を反転します
	void changeDirType(float direction);
	// アニメーションの時間を初期化します
	void initAnimeTime();
	// アニメーションの再生速度を変更します
	void setSpeed(float speed);
	// アニメーションをループさせるかを設定します
	void setIsLoop(bool isLoop);
	// 逆再生するかを設定します
	void setIsReverse(bool isReverse);
	// 
	void preMotion();
	void stopAnime();
	// アニメーションのタイムをランダムにします
	void timeRandom();
	// アニメーションの向きを変えたかを返します(1f)
	bool isBeginTurn();
	// 現在の再生フレームを取得します
	unsigned int getFrame();

public:
	int prevFrame_;			// 1f前のフレーム
	bool isLoop_;			// ループするか
	bool isStop_;			// アニメーションを止めるか
	bool isTurn_;			// 振り向きアニメーションをしたか
	bool isBeginTurn_;		// 振り向きアニメーションをしたか(1f)
	bool isReverse_;		// 逆再生するか
};


#endif
