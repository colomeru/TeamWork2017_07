#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/Hanger/Hanger.h"
#include "../../actor/Field/Clothes/UpHanger/UpHanger.h"
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
#include "../../game/Random.h"

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

	spriteIdMap_[CLOTHES_ID::THIN_CLOTHES].push_back(SPRITE_ID::TOWEL_CLOTHES_SPRITE);

	spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::NOT_SHAKE_CLOTHES].push_back(SPRITE_ID::NOT_SHAKE_CLOTHES_02_SPRITE);

	spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::NOT_SLASH_CLOTHES].push_back(SPRITE_ID::NOT_SLASH_CLOTHES_02_SPRITE);

	spriteIdMap_[CLOTHES_ID::TUTORIAL_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_SPRITE);
	spriteIdMap_[CLOTHES_ID::TUTORIAL_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_02_SPRITE);
	spriteIdMap_[CLOTHES_ID::TUTORIAL_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_03_SPRITE);
	spriteIdMap_[CLOTHES_ID::TUTORIAL_CLOTHES].push_back(SPRITE_ID::BASE_CLOTHES_04_SPRITE);
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
	//それぞれの服のポイントの設定
	pointSetter_.Load("BaseClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::BASE_CLOTHES_SPRITE, 125.0f);
	pointSetter_.Load("FluffyClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::FLUFFY_SPRITE, 125.0f);
	pointSetter_.Load("ThinClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::TOWEL_CLOTHES_SPRITE, 125.0f);
	pointSetter_.Load("NotShakeClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::NOT_SHAKE_CLOTHES_SPRITE, 125.0f);
	pointSetter_.Load("NotSlashClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::NOT_SLASH_CLOTHES_SPRITE, 125.0f);
	AddStage();
}

//ステージ追加
void Stage1::AddStage()
{
	auto row = csvReader_.rows();			//全体の行数
	auto col = csvReader_.columns();		//全体の列数
	int laneNum = 0;
	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
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
	//ステージ読み込み
	csvReader_.load("res/csv/" + fileName_ + ".csv");
	//服の当たり判定読み込み
	pointSetter_.Load("BaseClothesPoint");
	pointSetter_.SetLocalPoints(SPRITE_ID::BASE_CLOTHES_SPRITE, 125.0f);

	auto row = csvReader_.rows();			//全体の行数
	auto col = csvReader_.columns();		//全体の列数
	int laneNum = 0;
	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
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
	//0番の場合は何も生成しない
	if (data == 0) return;
	//6番の場合はハンガー生成
	if (data == 6) {
		world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<Hanger>(world_, CLOTHES_ID::HANGER, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE));
		return;
	}
	//8番の場合は上に上がるハンガー生成
	if (data == 8) {
		world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<UpHanger>(world_, CLOTHES_ID::FLUFFY_CLOTHES, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE));
		return;
	}

	//服の重さ、ID、画像のIDを決定する
	float weight = Random::GetInstance().Range(0.0f, 1.5f);
	CLOTHES_ID clothesId = (CLOTHES_ID)data;
	int sRand = Random::GetInstance().Range(0, spriteIdMap_[clothesId].size());
	SPRITE_ID id = spriteIdMap_[clothesId][sRand];
	std::pair<CLOTHES_ID, SPRITE_ID> pair{ clothesId, id };

	//csvに入力された番号ごとに服生成
	switch (data)
	{
	case 1: {
		auto base = std::make_shared<BaseClothes>(
			world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, weight, pair, pointSetter_.GetLocalPoints(id) , pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, base);
		if(pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), base.get(), base->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 2: {
		auto fluffy = std::make_shared<FluffyClothes>(
			world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, weight, pair, pointSetter_.GetLocalPoints(id), pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, fluffy);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), fluffy.get(), fluffy->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 3: {
		auto thin = std::make_shared<ThinClothes>(world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, weight, pair, pointSetter_.GetLocalPoints(id), pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, thin);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), thin.get(), thin->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 4: {
		auto notShake = std::make_shared<NotShakeClothes>(
			world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, 0.0f, pair, pointSetter_.GetLocalPoints(id), pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, notShake);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), notShake.get(), notShake->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 5: {
		auto notSlash = std::make_shared<NotSlashClothes>(
			world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, weight, pair, pointSetter_.GetLocalPoints(id), pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, notSlash);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), notSlash.get(), notSlash->GetFulcrum()));
		pin_list.pop();
		break;
	}
	case 7: {
		auto tutorial = std::make_shared<TutorialClothes>(
			world_, laneNum, Vector2(j, 0.0f) * STAGE_TIP_SIZE, weight, pair, pointSetter_.GetLocalPoints(id), pin_list.front());
		world_->Add(ACTOR_ID::STAGE_ACTOR, tutorial);
		if (pin_list.front())
			world_->Add(ACTOR_ID::PIN_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(50.0f, 50.0f), tutorial.get(), tutorial->GetFulcrum()));
		pin_list.pop();
		break;
	}
	default:
		break;
	}
}

void Stage1::GoalClothes_Add(int i, int j, int data, int laneNum)
{
	switch (data)
	{
	case 1: {
		world_->Add(ACTOR_ID::GOAL_ACTOR, std::make_shared<GoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, 0, Vector2(stageSize_.x, -600.0f)));
		break;
	}
	case 2: {
		world_->Add(ACTOR_ID::GOAL_ACTOR, std::make_shared<MoveGoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, 0, Vector2(stageSize_.x, -600.0f)));
		break;
	}
	}
}
