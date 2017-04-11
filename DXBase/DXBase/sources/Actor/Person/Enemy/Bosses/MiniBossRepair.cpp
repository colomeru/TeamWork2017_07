#include "MiniBossRepair.h"
#include "../../../../World/World.h"
#include "../../../Base/ActorGroup.h"
#include "../../../../Define.h"
#include "../../../../Game/Time.h"
#include "../../../../ResourceLoader/ResourceLoader.h"
MiniBossRepair::MiniBossRepair(IWorld * world, const Vector2& position, int num) :
	Actor(world, "MiniBossRepair", position, CollisionBase()),
	mNum(num),
	repairFlag(false),
	mLerpCount(0.0f),
	mTes(true)
{
	for (const auto& i : world->findActors(ActorGroup::Field)) {
		if (i->getName() == "BlockParticle") {
			mPoss.push_back(i->getPosition());
		}
	}
	mBlockToPos = mPoss[num] + Vector2(CHIPSIZE / 2, CHIPSIZE / 2);

	int spawnRand = GetRand(2);
	Vector2 pos;
	switch (spawnRand)
	{
	case 0: {
		pos = Vector2(-256, GetRand(SCREEN_SIZE.y));
		mChildDirection = true;
		break;
	}
	case 1: {
		pos = Vector2(SCREEN_SIZE.x + 256, GetRand(SCREEN_SIZE.y));
		mChildDirection = false;
		break;
	}
	case 2: {
		pos = Vector2(GetRand(SCREEN_SIZE.x), SCREEN_SIZE.y + 256);
		Vector2 vec = mBlockToPos - pos;
		if(vec.x>=0.0f) mChildDirection = true;
		else mChildDirection = false;
		break;
	}
	}
	mFirstPos = pos;
	position_ = pos;
	mBlockPos = position_;

	mAnimSize = ResourceLoader::GetInstance().GetTextureSize(AnimationID::BOSS_WAIT_TEX) / 2;
	mAnim.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::BOSS_WAIT_TEX));
	mAnim.change_param(0);
}

MiniBossRepair::~MiniBossRepair()
{
}

void MiniBossRepair::onUpdate(float deltaTime)
{
	if (!repairFlag) {
		mLerpCount -= 0.5f*Time::GetInstance().deltaTime();
	}
	else {
		mLerpCount += 0.5f*Time::GetInstance().deltaTime();
	}



	mLerpCount = MathHelper::Clamp(mLerpCount, 0.0f, 1.0f);
	position_ = Vector2::Lerp(mFirstPos, mBlockToPos, mLerpCount);

	mAnim.update(deltaTime);
}

void MiniBossRepair::onDraw() const
{
	mAnim.drawTurn(position_, mAnimSize, 0.5f, 0, Vector3(255, 255, 255),mChildDirection);
	//DrawGraph(position_.x, position_.y, ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STAGE1_TEX), true);
}

void MiniBossRepair::onCollide(Actor & actor)
{
}

void MiniBossRepair::Repair(bool flag)
{
	repairFlag = flag;
	if (mTes&&!flag) {
		mChildDirection = !mChildDirection;
		mTes = false;
	}
}
