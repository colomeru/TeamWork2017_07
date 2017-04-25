#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"
#include "../../actor/Field/Clothes/Hanger/Hanger.h"
#include "../../actor/Field/Clothes/GumClothes/GumClothes.h"
#include "../../actor/Field/Clothes/FluffyClothes/FluffyClothes.h"
#include "../../actor/Field/Clothes/ThinClothes/ThinClothes.h"

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

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			auto data = csvReader_.geti(i, j);
			auto lane_num = i / 4;
			switch (data)
			{
			case 1:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_, CLOTHES_ID::BASE_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 2:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<TestClothes>(world_, CLOTHES_ID::TEST_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 3:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<Hanger>(world_, CLOTHES_ID::HANGER, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 4:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<GumClothes>(world_, CLOTHES_ID::GUM_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 5:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<FluffyClothes>(world_, CLOTHES_ID::FLUFFY_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 6:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<ThinClothes>(world_, CLOTHES_ID::THIN_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 7:
				goal_ = std::make_shared<GoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, lane_num, Vector2(j, 0) * STAGE_TIP_SIZE);
				world_->Add(ACTOR_ID::STAGE_ACTOR, goal_);
				break;
			default:
				break;
			}
		}
	}

	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;
}
