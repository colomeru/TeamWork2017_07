#pragma once

#include "../Base/IScene.h"

class CreditScene : public IScene {
public:
	CreditScene(SceneDataKeeper* keeper);
	~CreditScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

private:
	int id;
	int sinCount;
	int timeCount;
	int MovieGraphHandle;

	int creditPosY;

	bool isPlay_;
};

