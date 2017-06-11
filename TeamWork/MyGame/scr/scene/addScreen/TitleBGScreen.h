#pragma once
#include <vector>
#include <map>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"../../stageGenerator/Stage.h"
#include"../../game/ID.h"

class TitleBGScreen
{
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
private:
	
	int currentStage_;
	
	int timeCount_;

	int plusCount_;
	int scrollPos_;
	float whiteScreenAlpha_;
	float whiteScreenAlphaAdd_;
	std::map<int, SPRITE_ID> BGList_;
	std::vector<int> lanePos_;
};