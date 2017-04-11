#ifndef ACTOR_PTR_H_
#define ACTOR_PTR_H_

#include <memory>

class Actor;
class ActorUI;
using ActorPtr = std::shared_ptr<Actor>;
using ActorUIPtr = std::shared_ptr<ActorUI>;
#endif
