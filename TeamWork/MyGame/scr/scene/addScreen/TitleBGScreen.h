#pragma once
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
	std::vector<int> lanePos_;
	std::map<SPRITE_ID,int> clothesAddPos_;
	std::vector<TitleLaneClothes> clothes_;
	std::vector<SPRITE_ID> clotheslist_;
};