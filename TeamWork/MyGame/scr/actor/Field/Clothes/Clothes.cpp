#include "Clothes.h"

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes)
	:Actor(world)
	,isHit_(false)
{
}
