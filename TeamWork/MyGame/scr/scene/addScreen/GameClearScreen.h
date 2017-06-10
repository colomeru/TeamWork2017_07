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

	void SetScore(int score) {
		score_ = score;
	}
	void SetHeadCount(int count) {
		headCount_ = count;
	}
	void SetStarCount() {
		starCount_ = 1;
		if (score_ >= 5000) {
			starCount_++;
		}
		if (headCount_ >= 5) {
			starCount_++;
		}
		dstar_.SetStarCount(starCount_);
	}
private:
	void ScoreUpdate();
	void ScoreDraw()const;
	void drawUpdate();
private:
	int score_;
	int headCount_;
	int starCount_;
	int inputCount_;
	int sinCount_;
	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::vector<Vector2> cursorPos_;
	Vector2 cursorDrawPos_;

	DrawStar dstar_;

	bool isShowScore_;
};