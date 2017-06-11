#pragma once
#include <vector>
#include <map>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\DrawStar.h"

class GameClearScreen
{
public:
	// コンストラクタ
	GameClearScreen();

	void Init();
	bool Update(Scene& nextScene);
	// 描画
	void Draw() const;

	void SetScore(int score, int count);
	void SetHeadCount();
	void SetStarCount();
	void SetFullStarCount();
private:
	void ScoreUpdate();
	void ScoreDraw()const;
	void drawUpdate();
private:
	int score_;
	float fscore_;
	int headCount_;
	float fheadCount_;
	int starCount_;
	int inputCount_;
	int sinCount_;
	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::vector<Vector2> cursorPos_;
	Vector2 cursorDrawPos_;

	DrawStar dstar_;
	bool isHeadDraw_;
	bool isShowScore_;
	bool isKeyOnceScore_;
};