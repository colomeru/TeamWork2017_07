#pragma once

#include "../Base/IScene.h"
#include"../../ResourceLoader/ResourceLoader.h"

class GameEndScene : public IScene {
public:
	GameEndScene(SceneDataKeeper* keeper);
	~GameEndScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

private:
	int sinCount;
	Vector2 textPos;
	std::map<int, Vector2> shotPos;
	std::map<int, int> defposlist;
	std::map<int, Vector2> lastPoses;
	std::map<int, Vector2> setPoses;

};

