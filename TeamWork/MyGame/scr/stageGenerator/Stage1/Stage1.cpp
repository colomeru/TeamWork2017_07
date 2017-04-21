#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"


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
	auto row = csvReader_.rows();
	auto col = csvReader_.columns();

	//csvの行(row)と列(col)から位置を出し、オブジェクト判別し、生成
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			auto data = csvReader_.geti(i, j);
			switch (data)
			{
			case 1:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_, CLOTHES_ID::BASE_CLOTHES, 0, Vector2(j, i) * 32));
				break;
			case 2:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<TestClothes>(world_, CLOTHES_ID::TEST_CLOTHES, 0, Vector2(j, i) * 32));
				break;
			default:
				break;
			}
		}
	}

	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;
}

//ステージサイズ取得
Vector2 Stage1::GetStageSize() const
{
	return stageSize_;
}
