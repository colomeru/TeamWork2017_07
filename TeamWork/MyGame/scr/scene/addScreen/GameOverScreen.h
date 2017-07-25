#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"

class GameOverScreen
{
public:
	// �R���X�g���N�^
	GameOverScreen();
	~GameOverScreen();
	void Init();
	bool Update(Scene& nextScene);
	// �`��
	void Draw() const;

private:
	void drawUpdate();
private:
	int inputCount_{ 0 };
	int sinCount_;
	bool isTrigger_{ true };

	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::vector<Vector2> cursorPos_;
	Vector2 cursorDrawPos_;
};