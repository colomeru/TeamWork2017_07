#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"
#include "../../actor/Field/Clothes/Hanger/Hanger.h"
#include "../../actor/Field/Clothes/GumClothes/GumClothes.h"
#include "../../actor/Field/Clothes/FluffyClothes/FluffyClothes.h"
#include "../../actor/Field/Clothes/ThinClothes/ThinClothes.h"
#include "../../actor/Field/Clothes/Hairball/HairballGenerator/HairballGenerator.h"
#include "../../actor/Field/Clothes/GoalClothes/GoalClothes.h"
#include "../../actor/Field/ClothesPin.h"

//コンストラクタ
Stage1::Stage1(IWorld * world, std::string & fileName)
	:StageGenerator(world, fileName)
{
}

//デストラクタ
Stage1::~Stage1()
{
}

//ステージ読み込み
void Stage1::LoadStage()
{
	csvReader_.load("res/csv/" + fileName_ + ".csv");
	AddStage();
}

//ステージ追加
void Stage1::AddStage()
{
	auto row = csvReader_.rows();			//全体の行数
	auto col = csvReader_.columns();		//全体の列数
	int laneNum = 0;

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
		//laneNum = i / 5;
		auto pin = i % 5;
		if (pin == 0 && i != 0) {
			laneNum++;
			while(!pin_list.empty())
			pin_list.pop();

		}
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
				break;
			}
			}

		}
	}


	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;
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
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_, CLOTHES_ID::BASE_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, pin_list.front()));
		pin_list.pop();
		break;
	}
	case 2: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<TestClothes>(world_, CLOTHES_ID::TEST_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, pin_list.front()));
		pin_list.pop();
		break;
	}
	case 3: {
		world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<Hanger>(world_, CLOTHES_ID::HANGER, laneNum, Vector2(j, -0.4f) * STAGE_TIP_SIZE));
		break;
	}
	case 4: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<GumClothes>(world_, CLOTHES_ID::GUM_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, pin_list.front()));
		pin_list.pop();
		break;
	}
	case 5: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<FluffyClothes>(world_, CLOTHES_ID::FLUFFY_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, pin_list.front()));
		pin_list.pop();
		break;
	}
	case 6: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<ThinClothes>(world_, CLOTHES_ID::THIN_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE, pin_list.front()));
		pin_list.pop();
		break;
	}
	case 7: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<GoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
		break;
	}
	case 8: {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<HairballGenerator>(world_, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
		break;
	}
	}

}
