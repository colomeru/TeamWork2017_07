#pragma once
#include "GameFrame.h"
#include "Content.h"
#include "../scene/SceneManager.h"
#include "../time/Time.h"

class Game1 : public GameFrame
{
public:
	Game1() = default;
	
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;
	virtual bool IsWait() const
	{
		return mIsLoaded;
	}

private:
	SceneManager	mSceneManager;
	Content			mContent;
	Time			mTime;

	bool			mIsLoaded;
};