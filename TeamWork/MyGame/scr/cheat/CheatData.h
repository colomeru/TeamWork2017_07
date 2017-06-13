#pragma once
#include<vector>
#include"../stageGenerator/Stage.h"

class CheatData {
private:
	CheatData():selectStage_(Stage::Stage1),startStageNum_(0) {
		clearStage.resize(8, false);
	}
	~CheatData(){}

public:
	static CheatData& getInstance() {
		static CheatData cheat;
		return cheat;
	}
	void SetClearData(int stage, bool is) {
		clearStage[stage] = is;
	}
	bool GetClearData(int stage)const {
		return clearStage.at(stage);
	}
	void SetSelectStage(Stage s){
		selectStage_ = s;
	}
	Stage GetSelectStage()const {
		return selectStage_;
	}
	void SetStartStage(const int s) {
		startStageNum_ = s;
	}
	int GetStartStage()const {
		return startStageNum_;
	}

private:
	std::vector<bool> clearStage;

	Stage selectStage_;

	int startStageNum_;
};