#ifndef _ISCENE_PTR_H_
#define _ISCENE_PTR_H_

#include <memory>

//シーンポインタ
class IScene;
using IScenePtr = std::shared_ptr<IScene>;

#endif