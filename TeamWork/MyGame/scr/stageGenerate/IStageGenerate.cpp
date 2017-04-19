#include "IStageGenerate.h"

//コンストラクタ
IStageGenerate::IStageGenerate(IWorld * world, const std::string & fileName)
	:world_(world)
	,fileName_(fileName)
{
}

IStageGenerate::~IStageGenerate()
{
}
