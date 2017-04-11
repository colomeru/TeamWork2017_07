#pragma once
#include"../Base/IScene.h"
#include"../../ResourceLoader/ResourceLoader.h"

class TeamLogoScene : public IScene {
public:
	TeamLogoScene(SceneDataKeeper* keeper);
	~TeamLogoScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

private:
	float timeCount;
};
