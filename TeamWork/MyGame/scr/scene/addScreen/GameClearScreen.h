#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"

class GameClearScreen
{
public:
	// �R���X�g���N�^
	GameClearScreen();

	void Init();
	bool Update(Scene& nextScene);
	// �`��
	void Draw() const;

private:
	void drawUpdate();
private:
	int inputCount_;
	int sinCount_;
	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::vector<Vector2> cursorPos_;
	Vector2 cursorDrawPos_;

};