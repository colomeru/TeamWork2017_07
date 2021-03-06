#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"../../stageGenerator/Stage.h"
#include "screenSupport/ProgressMeter.h"

class World;
class UIScreen
{
public:
	// コンストラクタ
	UIScreen() {}
	UIScreen(World* world);

	void Init(Stage currentStage,float stageLen);
	void Update(const Vector2& playerPos);
	// 描画
	void Draw() const;
	void End();
	void AddScore(int score);
	int GetScore()const {
		return (iscore_);
	}
private:
	World* world_;

	Stage currentStage_;
	int timeCount_;
	
	float meterLen_;
	Vector2 meterPos_;
	float stageLen_;
	Vector2 playerPos_;
	float fscore_;
	int iscore_;

	ProgressMeter meter_;
};