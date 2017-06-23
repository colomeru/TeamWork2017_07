#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"

class PauseScreen
{
public:
	enum returnGameType {
		Retry=0,
		Resume=1
	};
public:
	// �R���X�g���N�^
	PauseScreen();

	void Init();
	bool Update(Scene& nextScene,returnGameType& type);
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