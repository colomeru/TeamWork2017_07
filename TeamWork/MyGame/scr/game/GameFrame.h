#pragma once
#include "../time/FPS.h"


class GameFrame
{
public:
	GameFrame() = default;
	~GameFrame() = default;

	void Run();
	bool IsEnd();
	static void GameEnd();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	virtual bool IsWait() const
	{
		return false;
	}

private:
	static bool mIsEnd;

	FPS			fps;
};