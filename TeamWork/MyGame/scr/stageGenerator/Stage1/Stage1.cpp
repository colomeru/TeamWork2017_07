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
			auto laneNum = i / 4;				//���[���ԍ�
			auto pin_cond = i % 4;				//�s���p�̏���
			switch (data)
			{
			case 1:
				switch (pin_cond)
				{
				case 0:
					world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_, CLOTHES_ID::BASE_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
					break;
				case 1:
					world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<ClothesPin>(world_, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE + STAGE_PIN_POS_CV));
					break;
				}
				break;
			case 2:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<TestClothes>(world_, CLOTHES_ID::TEST_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 3:
				world_->Add(ACTOR_ID::HANGER_ACTOR, std::make_shared<Hanger>(world_, CLOTHES_ID::HANGER, laneNum, Vector2(j, -0.4f) * STAGE_TIP_SIZE));
				break;
			case 4:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<GumClothes>(world_, CLOTHES_ID::GUM_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 5:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<FluffyClothes>(world_, CLOTHES_ID::FLUFFY_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 6:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<ThinClothes>(world_, CLOTHES_ID::THIN_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 7:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<GoalClothes>(world_, CLOTHES_ID::GOAL_CLOTHES, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
				break;
			case 8:
				world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<HairballGenerator>(world_, laneNum, Vector2(j, 0) * STAGE_TIP_SIZE));
			default:
				break;
			}
		}
	}

	stageSize_ = Vector2(col, row) * STAGE_TIP_SIZE;
}
