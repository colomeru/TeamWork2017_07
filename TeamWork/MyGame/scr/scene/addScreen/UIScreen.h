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
	UIScreen()=default;
	explicit UIScreen(World* world);

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

	Stage currentStage_{ Stage::Stage1 };
	int timeCount_{0};
	
	float meterLen_{ 800.0f };
	Vector2 meterPos_{ Vector2(1100.0f, 50.0f) };
	float stageLen_{0.0f};
	Vector2 playerPos_{ Vector2::Zero };
	float fscore_{ 0.0f };
	int iscore_{ 0 };

	ProgressMeter meter_{};
};