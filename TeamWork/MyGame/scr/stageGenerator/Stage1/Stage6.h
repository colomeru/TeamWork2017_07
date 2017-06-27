#pragma once
#include "../Stage1\Stage1.h"

class Stage6 : public Stage1
{
public:
	Stage6(IWorld* world, std::string& fileName)
		:Stage1(world, fileName)
	{
		spriteIdMap_.clear();
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_07_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_12_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_13_SPRITE);

		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_02_SPRITE);
		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_04_SPRITE);
		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_06_SPRITE);

		spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_02_SPRITE);

		spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_02_SPRITE);

	}
};