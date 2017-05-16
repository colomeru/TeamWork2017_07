#include "StageGenerator.h"

//コンストラクタ
StageGenerator::StageGenerator(IWorld * world, const std::string & fileName)
	:world_(world)
	,fileName_(fileName)
	,STAGE_TIP_SIZE(128)
	,STAGE_PIN_POS_CV(75, -75)
{
}
  