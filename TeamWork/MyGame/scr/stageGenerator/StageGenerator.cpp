#include "StageGenerator.h"

//コンストラクタ
StageGenerator::StageGenerator(IWorld * world, const std::string & fileName)
	:world_(world)
	,fileName_(fileName)
	,stageSize_(Vector2())
	,STAGE_TIP_SIZE(128)
{
}
  