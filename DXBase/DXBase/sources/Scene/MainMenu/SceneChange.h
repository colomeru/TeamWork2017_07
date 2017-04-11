#pragma once

#include<map>
#include<vector>
#include"../../Math/Vector2.h"
#include<random>
#include"../Base/Scene.h"
#include"SceneChangeAnm.h"
#include"../StageClear/StageClearTextAnm.h"

static const int fadeTime = 100;

struct ChangeSColor
{
	int Red;
	int Green;
	int Blue;
	ChangeSColor() {

	}
	ChangeSColor(int R,int G,int B) {
		Red = R;
		Green = G;
		Blue = B;
	}
};
class SceneChange {
public:
	SceneChange();
	void start(std::string next, std::string tonexScene);
	void update();
	void fallSlimes();
	void draw()const;
	bool getEnd()const {
			return isEnd_;
		
	}


	bool getIsSlimeMax()const {
			return isSlimeMax_;
	}
private:
	bool isSlimeMax_;
	bool isEnd_;

	int slimesetline;
	float deltaTime_;
	int slimeCount;
	int switchCount;
	float timeCount;
	std::vector<int> numberMap;
	std::map<int, Vector2> slimes;
	std::map<int, ChangeSColor> slimeColors;
	std::map<int, std::map<int, Vector2>> spawnPoses;
	std::map<int, bool> isArrive_;
	int allSizes;
	
	std::random_device random_;
	std::mt19937 mt;
	std::uniform_int_distribution<> randR;
	std::uniform_int_distribution<> randG;
	std::uniform_int_distribution<> randB;
	std::uniform_int_distribution<> rand256;
	std::uniform_int_distribution<> randL;
	
	std::vector<std::vector<std::uniform_int_distribution<>>> randMaster;
	std::vector<int> useRand_;
	//MainMenu以外のシーンをフェードイン,アウトさせるための値
	int changeCount;
	int soundVol;
	bool isMainMenu_;

	SceneChangeAnm anmer_;
};