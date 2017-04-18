#pragma once

#include <memory>

class IStageGenerate;
// シーン用のシェアドポインタ
using IStageGeneratePtr = std::shared_ptr<IStageGenerate>;