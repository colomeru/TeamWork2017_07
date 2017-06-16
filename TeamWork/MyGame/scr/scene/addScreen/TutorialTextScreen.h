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
	TutorialTextScreen() {}
	TutorialTextScreen(World* world);

	void Init(const std::string& filename);
	//void Update();
	bool TutorialUpdate();
	// 描画
	void Draw() const;
	void End();
private:
	int GetMaxTextSize() {
		int ret = 0;
		for (int i = currentTextLine_ * 3; i< currentTextLine_ * 3 + 3; i++) {
			if (i >= drawText_.size())break;
			ret+=drawText_[i].size();
		}
		return ret;
	}
private:
	World* world_;

	Stage currentStage_;

	int textCount_;
	int sinCount_;
	int maxTextSize_;

	int currentTextLine_;

	float textChangeTimeCount_;

	std::vector<std::vector<char>> drawText_;
};