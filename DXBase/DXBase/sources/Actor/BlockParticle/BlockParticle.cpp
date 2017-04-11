#include "BlockParticle.h"
#include "../../Game/Time.h"
#include "../../ResourceLoader/ResourceLoader.h"
#include "../../Define.h"
BlockParticle::BlockParticle(IWorld * world, const Vector2 & position) :
	Actor(world, "BlockParticle", position, CollisionBase(
		Vector2{ position.x,position.y },
		Vector2{ position.x - (CHIPSIZE),position.y },
		Vector2{ position.x ,position.y - (CHIPSIZE) },
		Vector2{ position.x - (CHIPSIZE),position.y - (CHIPSIZE) })),
	mAngle(0.0f),
	mVelo(Vector2::Zero),
	mBreakFlag(false),
	mPosition(position),
	mRepairFlag(false)
{
	mDrawPos = position;
	float rand = (float)(GetRand(200) - 100);
	mVelo = Vector2(rand, -50.0f);
	mAngleVelo = (float)(GetRand(20) - 10) / 10.0f;
}

BlockParticle::~BlockParticle()
{
}

void BlockParticle::onUpdate(float deltaTime)
{
	if (!mBreakFlag || mRepairFlag)return;
	float delta = Time::GetInstance().deltaTime()*4.0f;
	//‰º‚É—Ž‚¿‚éˆ—
	mVelo.y += 75.0f*delta;
	//‘¬“x‚ð‘«‚·
	mDrawPos += mVelo*delta;
	//Šp“x‚ð‘«‚·
	mAngle += mAngleVelo*delta;
}

void BlockParticle::onDraw() const
{

	if (!mRepairFlag) {
		//‹ts—ñ‚ðŠ|‚¯‚é
		Vector3 invPos = Vector3(mDrawPos.x, mDrawPos.y, 0.0f)*world_->GetInv();
		DrawRotaGraph(invPos.x + 48, invPos.y + 48, 1.0f, mAngle, ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STAGE4_TEX), true);
	}
	else {
		Vector3 invPos = Vector3(mPosition.x, mPosition.y, 0.0f)*world_->GetInv();
		DrawRotaGraph(invPos.x + 48, invPos.y + 48, 1.0f, 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_STAGE2_TEX), true);
	}

}

void BlockParticle::onCollide(Actor & other)
{
}

void BlockParticle::Break(bool flag)
{
	mBreakFlag = flag;
}

void BlockParticle::RepairBlock(bool flag)
{
	mRepairFlag = flag;
}

Vector2 BlockParticle::GetFirstPosition()
{
	return mPosition;
}
