#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"
#include "../../actor/Field/Clothes/Hanger/Hanger.h"
#include "../../actor/Field/Clothes/UpHanger/UpHanger.h"
#include "../../actor/Field/Clothes/GumClothes/GumClothes.h"
#include "../../actor/Field/Clothes/FluffyClothes/FluffyClothes.h"
#include "../../actor/Field/Clothes/ThinClothes/ThinClothes.h"
#include "../../actor/Field/Clothes/Hairball/HairballGenerator/HairballGenerator.h"
#include "../../actor/Field/Clothes/GoalClothes/GoalClothes.h"
#include "../../actor/Field/Clothes/GoalClothes/MoveGoalClothes.h"
#include "../../actor/Field/Clothes/StartClothes/StartClothes.h"
#include "../../actor/Field/Clothes/NotShakeClothes/NotShakeClothes.h"
#include "../../actor/Field/Clothes/NotSlashClothes/NotSlashClothes.h"
#include "../../actor/Field/Clothes/TutorialClothes/TutorialClothes.h"
#include "../../actor/Field/ClothesPin.h"
#include "../MyGame/scr/game/Random.h"

//コンストラクタ
Stage1::Stage1(IWorld * world, std::string & fileName, int frequencyWind, int frequencyHairball, int hairballCnt)
	:StageGenerator(world, fileName)
	,frequencyWind_(frequencyWind), frequencyHairball_(frequencyHairball), hairballCnt_(hairballCnt), currentStage_(Stage::Stage1)
{
	spriteIdMap_.clear();
	spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_02_SPRITE);
	spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_03_SPRITE);
	spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_04_SPRITE);

	spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_SPRITE);
	spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_02_SPRITE);
	spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].push_back(SPRITE_ID::FLUFFY_03_SPRITE);

	spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_02_SPRITE);

	spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_02_SPRITE);
}

//デストラクタ
Stage1::~Stage1()
{
	spriteIdMap_.clear();
}

//ステージ読み込み
void Stage1::LoadStage(Stage name)
{
	currentStage_ = name;
	csvReader_.load("res/csv/" + fileName_ + ".csv");
	AddStage();
}

//ステージ追加
void Stage1::AddStage()
{
	auto row = csvReader_.rows();			//全体の行数
	auto col = csvReader_.columns();		//全体の列数
	int laneNum = 0;
	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;

	//スタートの服の生成
	//world_->Add(ACTOR_ID::STAGE_ACTOR,std::make_shared<StartClothes>(
	//	world_, 
	//	CLOTHES_ID::START_CLOTHES, 
	//	world_->GetKeepDatas().playerLane_, 
	//	Vector2(STAGE_TIP_SIZE, 0), 
	//	0.5f, false));

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
		//laneNum = i / 5;
		auto pin = i % 5;
		if (pin == 0 && i != 0) {
			laneNum++;
			while(!pin_list.empty())
				pin_list.pop();

		}
		//csvの列番号によって生成するオブジェクトを変更
		for (int j = 0; j < col; j++) {
			auto data = csvReader_.geti(i, j);
			switch (pin)
			{

			case 1: {
				Pin_Add(i, j, data, laneNum);
				break;
			}
			case 2: {
				Clothes_Add(i, j, data, laneNum);
				world_->EachActor(ACTOR_ID::STAGE_ACTOR, [=](Actor& other){
					static_cast<Clothes*>(&other)->SetFrequencyWind(frequencyWind_);
					static_cast<Clothes*>(&other)->SetCurrentStage(currentStage_);
				});
				break;
			}
			case 3: {
				GoalClothes_Add(i, j, data, laneNum);
				break;
			}
			}
		}
	}

	//毛玉生成クラスの生成
	for (int i = 0; i < 3; i++) {
		world_->Add(ACTOR_ID::HAIRBALL_ACTOR, std::make_shared<HairballGenerator>(world_, i, Vector2::Zero, frequencyHairball_, hairballCnt_));
	}
}
void Stage1::CreateClothes() {
	csvReader_.load("res/csv/" + fileName_ + ".csv");

	auto row = csvReader_.rows();			//全体の行数
	auto col = csvReader_.columns();		//全体の列数
	int laneNum = 0;
	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
		//laneNum = i / 5;
		auto pin = i % 5;
		if (pin == 0 && i != 0) {
			laneNum++;
			while (!pin_list.empty())
				pin_list.pop();

		}
		for (int j = 0; j < col; j++) {
			if (pin != 2)break;
			pin_list.push(false);
			auto data = csvReader_.geti(i, j);
			Clothes_Add(i, j, data, laneNum);
		}
	}
}

void Stage1::Pin_Add(int i, int j, int data, int laneNum)
{
	switch (data)
	{
	case 1: {
		pin_list.push(true);
		break;
	}
	case 2:
		pin_list.push(false);
		break;
	}
}

void Stage1::Clothes_Add(int i, int j, int data, int laneNum)
{
	switch (data)
	{
	case 1: {
		int sRand = Random::GetInstance().Range(0, spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].size());
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto base = std::make_shared<BaseClothes>(
			world_, CLOTHES_ID::BASE_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, spriteIdMap_[CLOTHES_ID::BASE_CLOTHES][sRand], pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, base);
		if(pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), base.get(), base->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 2: {
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto test = std::make_shared<TestClothes>(world_, CLOTHES_ID::TEST_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, test);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), test.get(), test->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 3: {
		world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<Hanger>(world_, CLOTHES_ID::HANGER, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
		break;
	}
	case 4: {
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto gum = std::make_shared<GumClothes>(world_, CLOTHES_ID::GUM_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, gum);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), gum.get(), gum->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 5: {
		int sRand = Random::GetInstance().Range(0, spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES].size());
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto fluffy = std::make_shared<FluffyClothes>(
			world_, CLOTHES_ID::FLUFFY_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, spriteIdMap_[CLOTHES_ID::FLUFFY_CLOTHES][sRand], pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, fluffy);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), fluffy.get(), fluffy->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 6: {
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto thin = std::make_shared<ThinClothes>(world_, CLOTHES_ID::THIN_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, thin);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), thin.get(), thin->GetFulcrum()));
		pin_list.pop();
		break;
	}	
	case 7: {
		int sRand = Random::GetInstance().Range(0, spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].size());
		auto notShake = std::make_shared<NotShakeClothes>(
			world_, CLOTHES_ID::NOT_SHAKE_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, 0.0f, spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES][sRand], pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, notShake);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), notShake.get(), notShake->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 8: {
		int sRand = Random::GetInstance().Range(0, spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].size());
		auto notSlash = std::make_shared<NotSlashClothes>(
			world_, CLOTHES_ID::NOT_SLASH_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, 0.0f, spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES][sRand], pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, notSlash);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), notSlash.get(), notSlash->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 9: {
		world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<UpHanger>(world_, CLOTHES_ID::HANGER, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
		break;
	}
	case 10: {
		int sRand = Random::GetInstance().Range(0, spriteIdMap_[CLOTHES_ID::BASE_CLOTHES].size());
		float weight = Random::GetInstance().Range(0.0f, 1.5f);
		auto base = std::make_shared<TutorialClothes>(
			world_, CLOTHES_ID::BASE_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, weight, spriteIdMap_[CLOTHES_ID::BASE_CLOTHES][sRand], pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, base);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50, 50), base.get(), base->GetFulcrum()));
		pin_list.pop();
		break;
	}

	}

}

void Stage1::GoalClothes_Add(int i, int j, int data, int laneNum)
{
	switch (data)
	{
	case 1: {
		world_->Add(ACTOR_ID::GOAL_ACTOR, std::make_shared<GoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, 0, Vector2(stageSize_.x, -600)));
		break;
	}
	case 2: {
		world_->Add(ACTOR_ID::GOAL_ACTOR, std::make_shared<MoveGoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, 0, Vector2(stageSize_.x, -600)));
		break;
	}
	}
}
