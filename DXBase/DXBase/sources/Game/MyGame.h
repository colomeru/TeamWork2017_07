#ifndef _MY_GAME_H_
#define _MY_GAME_H_

#include "Game.h"
#include "../Scene/Base/SceneMgr.h"
#include "../Scene/Base/SceneDataKeeper.h"

class MyGame : public Game{
public:
	MyGame();
	~MyGame();
	void start();
	void update();
	void draw();
	void end();
private:
	SceneMgr sceneMgr_;
	SceneDataKeeper dataKeeper_;
};

#endif