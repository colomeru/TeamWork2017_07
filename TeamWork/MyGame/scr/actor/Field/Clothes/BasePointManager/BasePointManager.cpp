#include "BasePointManager.h"

BasePointManager::BasePointManager()
{
}

void BasePointManager::SetLocalPoints(SPRITE_ID spriteId, std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	switch (spriteId)
	{
	case SPRITE_ID::BASE_CLOTHES_SPRITE: {
		SetBase01(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_02_SPRITE: {
		SetBase02(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_03_SPRITE: {
		SetBase03(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_04_SPRITE: {
		SetBase04(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_05_SPRITE: {
		SetBase05(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_06_SPRITE: {
		SetBase06(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_07_SPRITE: {
		SetBase07(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_08_SPRITE: {
		SetBase08(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_09_SPRITE: {
		SetBase09(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_10_SPRITE: {
		SetBase10(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_11_SPRITE: {
		SetBase11(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_12_SPRITE: {
		SetBase12(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_13_SPRITE: {
		SetBase13(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_14_SPRITE: {
		SetBase14(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_15_SPRITE: {
		SetBase15(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_16_SPRITE: {
		SetBase16(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_17_SPRITE: {
		SetBase17(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_18_SPRITE: {
		SetBase18(localPoints, length);
		break;
	}
	case SPRITE_ID::BASE_CLOTHES_19_SPRITE: {
		SetBase19(localPoints, length);
		break;
	}
	}

}

void BasePointManager::SetBase01(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 45 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 35 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 45 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase02(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 40 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 45 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 60 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 0 + length, 0));
}

void BasePointManager::SetBase03(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 100 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 100 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 65 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 35 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 60 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase04(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-80, -5 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-80, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(80, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(80, -5 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-80, -5 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-80, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(80, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(80, -5 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-80, -5 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-80, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(80, 60 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(80, -5 + length, 0));
}

void BasePointManager::SetBase05(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-75, -10 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-75, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(75, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(75, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-75, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-75, 40 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(75, 25 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(75, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-75, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-75, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(75, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(75, -10 + length, 0));
}

void BasePointManager::SetBase06(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-80, -10 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-80, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(75, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(75, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-80, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-80, 45 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(75, 25 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(75, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-80, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-80, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(75, 45 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(75, -10 + length, 0));
}

void BasePointManager::SetBase07(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-70, -10 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-75, 95 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 95 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, -10 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-75, 45 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 25 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, -10 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-75, 25 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 45 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, -10 + length, 0));
}

void BasePointManager::SetBase08(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 25 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase09(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 0 + length, 0));
}

void BasePointManager::SetBase10(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 55 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 70 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase11(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-50, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-50, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(50, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(50, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-50, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-50, 55 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(50, 35 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(50, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-50, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-50, 35 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(50, 55 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(50, 0 + length, 0));
}

void BasePointManager::SetBase12(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-75, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(75, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 40 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 45 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase13(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-60, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 35 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 65 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(60, 0 + length, 0));
}

void BasePointManager::SetBase14(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 5 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-65, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 5 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-65, 60 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 45 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 5 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 5 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-65, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 60 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 5 + length, 0));
}

void BasePointManager::SetBase15(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(55, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 40 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(55, 25 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(55, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(55, 0 + length, 0));
}

void BasePointManager::SetBase16(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 0 + length, 0));
}

void BasePointManager::SetBase17(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(65, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 55 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 35 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(65, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 40 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(65, 0 + length, 0));
}

void BasePointManager::SetBase18(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 85 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-70, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 35 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-70, 35 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 55 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(70, 0 + length, 0));
}

void BasePointManager::SetBase19(std::map<CuttingState, std::vector<Vector3>>& localPoints, float length)
{
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(-55, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(55, 90 + length, 0));
	localPoints[CuttingState::Normal].push_back(Vector3(55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(-55, 50 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(55, 30 + length, 0));
	localPoints[CuttingState::RightUpSlant].push_back(Vector3(55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 0 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(-55, 30 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(55, 50 + length, 0));
	localPoints[CuttingState::LeftUpSlant].push_back(Vector3(55, 0 + length, 0));
}
