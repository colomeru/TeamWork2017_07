#pragma once
#include <memory>

class Actor;
// アクター用シェアドポインタ
using ActorPtr = std::shared_ptr<Actor>;