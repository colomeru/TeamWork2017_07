#pragma once
#include "../Stage1\Stage1.h"

class Stage5 : public Stage1
{
public:
	Stage5(IWorld* world, std::string& fileName, int frequencyWind = 0, int frequencyHairball = 0, int hairballCnt = 0)
		:Stage1(world, fileName)
	{
		frequencyWind_ = frequencyWind;
		frequencyHairball_ = frequencyHairball;
		hairballCnt_ = hairballCnt;

		spriteIdMap_.clear();
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_02_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_08_SPRITE);
		spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_16_SPRITE);

		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_04_SPRITE);
		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_05_SPRITE);
		spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_06_SPRITE);

		spriteIdMap_[CLOTHES_ID::THIN_CLOTHES].push_back(SPRITE_ID::TOWEL_CLOTHES_SPRITE);

		spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_02_SPRITE);

		spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE);
		spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_02_SPRITE);

	}
};