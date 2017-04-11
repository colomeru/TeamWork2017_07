#pragma once

#include <memory>

class Player;
using PlayerPtr = std::shared_ptr<Player>;

class PlayerBody;
using PlayerBodyPtr = std::shared_ptr<PlayerBody>;

class PlayerConnector;
using PlayerCntrPtr = std::shared_ptr<PlayerConnector>;

class PlayerBodyPoint;
using PlayerBodyPointPtr = std::shared_ptr<PlayerBodyPoint>;

class PlayerBodyCollider;
using PlayerBodyCollPtr = std::shared_ptr<PlayerBodyCollider>;

