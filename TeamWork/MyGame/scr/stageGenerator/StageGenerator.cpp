#include "StageGenerator.h"

//コンストラクタ
StageGenerator::StageGenerator(IWorld * world, const std::string & fileName)
	:world_(world)
	,fileName_(fileName)
{
}
