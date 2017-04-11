#pragma once

#include <memory>

class IScene;
// シーン用のシェアドポインタ
using IScenePtr = std::shared_ptr<IScene>;