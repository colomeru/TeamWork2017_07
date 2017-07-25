#pragma once
#include <vector>
#include <map>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\DrawStar.h"
#include"../../stageGenerator/Stage.h"

class AllClearScreen
{
public:
	// コンストラクタ
	AllClearScreen();
	~AllClearScreen();

	void Init();
	bool Update(Scene& nextScene);
	// 描画
	void Draw() const;

	void SetScore(int score, int count, Stage stage);
	void SetHeadCount();
	void SetStarCount();
	void SetFullStarCount();
private:
	void ScoreUpdate();
	void ScoreDraw()const;
	void drawUpdate();
private:
	int score_;
	float fscore_{ 0.f };
	int headCount_{ 0 };
	float fheadCount_{ 0.f };
	int starCount_{ 0 };
	int inputCount_{ 0 };
	int sinCount_;
	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::vector<Vector2> cursorPos_;
	Vector2 cursorDrawPos_;

	DrawStar dstar_;
	bool isHeadDraw_{ false };
	bool isShowScore_{ true };
	bool isKeyOnceScore_{ false };
	Stage stage_{ Stage::Stage1 };
};