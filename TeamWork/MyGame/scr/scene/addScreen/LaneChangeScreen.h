#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"


class World;
class LaneChangeScreen
{
public:
	// コンストラクタ
	LaneChangeScreen() {}
	LaneChangeScreen(World* world);

	void Init();
	void Update();
	// 描画
	void Draw() const;
	void End();
private:
	World* world_;

	int timeCount_;

	//std::vector<BackgroundCharacters*> characters_;

};