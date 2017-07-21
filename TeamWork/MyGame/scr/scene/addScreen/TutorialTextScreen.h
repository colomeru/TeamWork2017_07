#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"../../stageGenerator/Stage.h"
#include <iostream>
#include<fstream>

class World;
class TutorialTextScreen
{
public:
	// コンストラクタ
	TutorialTextScreen() = default;
	TutorialTextScreen(World* world);

	void Init(const std::string& filename);
	bool TutorialUpdate();
	// 描画
	void Draw() const;
	void End();
private:
	int GetMaxTextSize();
private:
	World* world_;

	int textCount_{ 0 };
	int sinCount_{ 0 };
	int maxTextSize_{ 0 };

	int currentTextLine_{ 0 };

	float textChangeTimeCount_{ 0.f };

	std::vector<std::vector<char>> drawText_;
};