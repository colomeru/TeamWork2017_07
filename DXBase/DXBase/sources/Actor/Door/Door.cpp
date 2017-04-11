#include "Door.h"
#include "../../Define.h"
#include "../../ResourceLoader/ResourceLoader.h"
#include "../../Game/Time.h"
#include "../../Define.h"
Door::Door(IWorld * world, const Vector2 & position) :
	Actor(world, "Door", position,
		CollisionBase(
			Vector2(position.x, position.y),
			Vector2(position.x - (CHIPSIZE), position.y),
			Vector2(position.x, position.y - (CHIPSIZE * 5)),
			Vector2(position.x - (CHIPSIZE), position.y - (CHIPSIZE * 4)))),
	mDoorFlag(false),
	mOpenFlag(false),
	mCloseFlag(true)
{

	mLoadAnim.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::DOOR_OPEN));
	mLoadAnim.add_anim(1, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::DOOR_CLOSE));

	mEndAnim = false;
	//アニメーション初期化
	mLoadAnim.change_param(1, 0.0f);



}

Door::~Door()
{
}

void Door::onUpdate(float deltaTime)
{
	if (mCloseFlag)body_.enabled(true);
	if (mOpenFlag)body_.enabled(false);

	if (mDoorFlag) {
		mCloseFlag = false;
		mLoadAnim.change_param(0, 1.0f);
		if (mLoadAnim.end_anim())
		{
			mOpenFlag = true;
			mLoadAnim.change_param(0, 0.0f);
		}
	}
	else {
		mOpenFlag = false;
		mLoadAnim.change_param(1, 1.0f);
		if (mLoadAnim.end_anim())
		{
			mCloseFlag = true;
			mLoadAnim.change_param(1, 0.0f);
		}
	}
	mLoadAnim.update(Time::GetInstance().deltaTime());
}

void Door::onDraw() const
{
	Vector2 pos = position_*inv_;

	if (!mEndAnim)
		mLoadAnim.draw(pos+Vector2(CHIPSIZE-16,0), Vector2::Zero, Vector2::One);
	if(mCloseFlag)
		DrawGraph(pos.x+CHIPSIZE-16, pos.y, ResourceLoader::GetInstance().getTextureID(TextureID::DOOR_STAY_TEX), true);
	if(mOpenFlag)
		DrawGraph(pos.x+CHIPSIZE-16, pos.y, ResourceLoader::GetInstance().getTextureID(TextureID::DOOR_OPEN_TEX), true);

	//body_.draw(inv_);
	//DrawBox(pos.x, pos.y, (pos + Vector2(CHIPSIZE, CHIPSIZE)).x, (pos + Vector2(CHIPSIZE, CHIPSIZE)).y, GetColor(255, 255, 255), TRUE);
}

void Door::onCollide(Actor & other)
{

}

void Door::DoorOpen(bool openFlag)
{
	mDoorFlag = openFlag;
}
