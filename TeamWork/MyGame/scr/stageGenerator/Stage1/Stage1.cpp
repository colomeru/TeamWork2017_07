#include "Stage1.h"
#include "../../game/ID.h"
#include "../../actor/Field/Clothes/BaseClothes.h"
#include "../../actor/Field/Clothes/TestClothes.h"


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
	auto row = csvReader_.rows();
	auto col = csvReader_.columns();

	//csv�̍s(row)�Ɨ�(col)����ʒu���o���A�I�u�W�F�N�g���ʂ��A����
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

//�X�e�[�W�T�C�Y�擾
Vector2 Stage1::GetStageSize() const
{
	return stageSize_;
}
