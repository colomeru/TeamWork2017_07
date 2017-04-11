#ifndef _SCENE_MGR_H_
#define _SCENE_MGR_H_

#include <unordered_map>
#include "Scene.h"
#include "IScenePtr.h"
#include"SceneDataKeeper.h"
#include"../MainMenu/SceneChange.h"

class SceneMgr {
public:
	SceneMgr(SceneDataKeeper* keeper);
	void init();
	void update();
	void draw() const;
	void end();
	void add(Scene name, const IScenePtr& scene);
	void change(Scene name);
	void sceneChanger();
	SceneMgr(const SceneMgr& other) = delete;
	SceneMgr operator = (const SceneMgr& other) = delete;

private:
	std::unordered_map<Scene, IScenePtr>scenes_;
	IScenePtr currentScene_;
	SceneDataKeeper* keeper_;
	SceneChange changer_;

	int absnum;

	bool isFirst_;
	bool isMove_;

};

#endif