#pragma once

#include"../Base/Scene.h"

class MoveScreen {
public:
	MoveScreen();
	bool update(std::string name, Scene& next);
	bool update(std::string name,Scene& next,bool& isClear);
	void draw() const;
private:
	std::vector<std::string> changeTextList;
	std::vector<std::vector<std::string>> listBase;
	std::vector<Vector2> textPosList;
};