#include "StageGenerator.h"

//�R���X�g���N�^
StageGenerator::StageGenerator(IWorld * world, const std::string & fileName)
	:world_(world)
	,fileName_(fileName)
	,stageSize_(Vector2())
	,STAGE_TIP_SIZE(128)
{
}
  