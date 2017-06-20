#include "FluffyPointManager.h"

FluffyPointManager::FluffyPointManager()
{
}

void FluffyPointManager::SetLocalPoints(SPRITE_ID spriteId, std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	switch (spriteId)
	{
	case SPRITE_ID::FLUFFY_SPRITE: {
		SetFluffy01(localPoints, length);
		break;
	}
	case SPRITE_ID::FLUFFY_02_SPRITE: {
		SetFluffy02(localPoints, length);
		break;
	}
	case SPRITE_ID::FLUFFY_03_SPRITE: {
		SetFluffy03(localPoints, length);
		break;
	}
	case SPRITE_ID::FLUFFY_04_SPRITE: {
		SetFluffy04(localPoints, length);
		break;
	}
	case SPRITE_ID::FLUFFY_05_SPRITE: {
		SetFluffy05(localPoints, length);
		break;
	}
	case SPRITE_ID::FLUFFY_06_SPRITE: {
		SetFluffy06(localPoints, length);
		break;
	}
	}
	
}

void FluffyPointManager::SetFluffy01(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void FluffyPointManager::SetFluffy02(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void FluffyPointManager::SetFluffy03(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void FluffyPointManager::SetFluffy04(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void FluffyPointManager::SetFluffy05(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void FluffyPointManager::SetFluffy06(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}
