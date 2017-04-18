#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"


//コンストラクタ
Stage1::Stage1(IWorld * world, std::string & fileName)
	:IStageGenerate(world, fileName)
{
}

Stage1::~Stage1()
{
}

void Stage1::LoadStage()
{
	csvReader_.load("res/csv/" + fileName_ + ".csv");
	AddStage();
}

void Stage1::AddStage()
{
	auto row = csvReader_.rows();
	auto col = csvReader_.columns();

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			auto data = csvReader_.geti(i, j);
			switch (data)
			{
			case 1:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_, Vector2(j, i) * 32));
			default:
				break;
			}
		}
	}
}
