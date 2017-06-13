#pragma once
#include <array>
#include <vector>
#include <map>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"../../stageGenerator/Stage.h"
#include"../../game/ID.h"

class TitleBGScreen
{
private:
	struct TitleLaneClothes{
		SPRITE_ID drawID;
		int laneNum;
		int lanePos;
		TitleLaneClothes(SPRITE_ID id, int num, int pos) :drawID(id), laneNum(num), lanePos(pos) {
		}
	};

public:
	// コンストラクタ
	TitleBGScreen();

	void Init();
	void Update();
	// 描画
	void Draw() const;
	void End();
	
private:
	void WhiteScreenMinus();
	void DrawClothes(int laneNum, int xpos,SPRITE_ID clothestype)const;
private:
	
	int currentStage_;
	
	int timeCount_;
	int clothesCreateTime_;

	int plusCount_;
	int scrollPos_;
	float whiteScreenAlpha_;
	float whiteScreenAlphaAdd_;
	std::map<int, SPRITE_ID> BGList_;
	int lanePos_[3];
	std::map<SPRITE_ID,int> clothesAddPos_;
	std::vector<TitleLaneClothes> clothes_;
	std::array<SPRITE_ID,2> clotheslist_;
};