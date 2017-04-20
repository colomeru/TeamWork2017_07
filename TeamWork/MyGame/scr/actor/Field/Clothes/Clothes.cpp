#include "Clothes.h"

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum)
	:Actor(world)
	,isHit_(false)
{
}
