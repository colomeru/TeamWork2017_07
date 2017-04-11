#ifndef _SCENE_NULL_H_
#define _SCENE_NULL_H_

#include "IScene.h"

class SceneNull : public IScene
{
public:
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;
};

#endif

