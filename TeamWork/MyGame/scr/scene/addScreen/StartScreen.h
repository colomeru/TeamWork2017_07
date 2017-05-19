#pragma once
#include"../Scene.h"
#include <vector>
#include<memory>

class StartCameraPointAct;
class World;

class StartScreen
{
public:
	// コンストラクタ
	StartScreen() {}
	StartScreen(World* world,int maxLaneCount);

	void Init(float stageWidth);
	bool Update();
	// 描画
	void Draw() const;

private:
	using CamPointPtr = std::shared_ptr<StartCameraPointAct>;

	CamPointPtr cameraPointActor_;
	World* world_;
	int maxLaneCount_;
};