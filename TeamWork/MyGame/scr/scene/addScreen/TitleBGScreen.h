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
		float rotate;
		float maxAngle;
		float windPower;
		float sinCount_;
		Vector2 clothOrigin;

		TitleLaneClothes(SPRITE_ID id, int num, int pos,float rotate,float maxAngle,float windPower,const Vector2& origin) :
			drawID(id),maxAngle(maxAngle),windPower(windPower),
			laneNum(num), lanePos(pos), rotate(rotate),
			clothOrigin(origin), sinCount_(0.0f){
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
	void DrawClothes(const TitleLaneClothes& clothes)const;

private:
	
	int currentStage_{ 0 };
	
	int timeCount_{ 0 };
	int clothesCreateTime_{ 30 };

	int plusCount_{ 1 };
	int scrollPos_{ 0 };
	float whiteScreenAlpha_{ 0.f };
	float whiteScreenAlphaAdd_{ 0.f };
	std::map<int, SPRITE_ID> BGList_;
	int lanePos_[3]{-50,400,900};
	std::vector<TitleLaneClothes> clothes_;
	std::array<SPRITE_ID,19> clotheslist_;
};