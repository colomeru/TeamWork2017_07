#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"
#include"screenSupport\BGCharas\LaneChangeWind.h"

class World;
class LaneChangeScreen
{
public:
	// コンストラクタ
	LaneChangeScreen() = default;
	explicit LaneChangeScreen(World* world);

	void Init(WindDir dir);
	void Update();
	// 描画
	void Draw() const;
	void End();
private:
	World* world_;

	int timeCount_{ 0 };
	bool isCreateWind_{ false };

	WindDir dir_{ WindDir::UP };

	std::map<WindDir, float> windDefPoses_;


};