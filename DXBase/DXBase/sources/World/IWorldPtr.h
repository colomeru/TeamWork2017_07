#ifndef IWORLD_PTR_H_
#define IWORLD_PTR_H_

#include <memory>

// ワールドポインタ
class IWorld;
using IWorldPtr = std::shared_ptr<IWorld>;

#endif
