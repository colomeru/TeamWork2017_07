#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"
#include "../../actor/Field/Clothes/Hanger/Hanger.h"
#include "../../actor/Field/Clothes/GumClothes/GumClothes.h"
#include "../../actor/Field/Clothes/FluffyClothes/FluffyClothes.h"
#include "../../actor/Field/Clothes/ThinClothes/ThinClothes.h"

//�R���X�g���N�^
Stage1::Stage1(IWorld * world, std::string & fileName)
	:StageGenerator(world, fileName)
{
}

//�f�X�g���N�^
Stage1::~Stage1()
{
}

//�X�e�[�W�ǂݍ���
void Stage1::LoadStage()
{
	csvReader_.load("res/csv/" + fileName_ + ".csv");
	AddStage();
}

//�X�e�[�W�ǉ�
void Stage1::AddStage()
{
	auto row = csvReader_.rows();			//�S�̂̍s��
	auto col = csvReader_.columns();		//�S�̗̂�

	//csv�̍s(row)�Ɨ�(col)����ʒu���o���A�I�u�W�F�N�g���ʂ��A����
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
