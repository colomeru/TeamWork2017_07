#include "Smoke.h"
#include "../../ResourceLoader/ResourceLoader.h"
#include "../../Game/Time.h"
#include <algorithm>
#include "../../Define.h"
Smoke::Smoke(IWorld * world, const Vector2 & position) :
	mSmokeTime(0.0f),
	mIsSmoke(false),
	Actor(world, "Smoke", position, CollisionBase())
{
	mSmokeAnims.clear();
	mAnimSize = ResourceLoader::GetInstance().GetTextureSize(AnimationID::BOSS_EFFECT_BOKO_TEX) / 2;
}

Smoke::~Smoke()
{
}
bool Smoke::RemoveState(const AnimState & anim)
{
	return anim.alpha <= 0.0f;
}
void Smoke::onUpdate(float deltaTime)
{
	if (mIsSmoke) {
		mSmokeTime += Time::GetInstance().deltaTime();
		if (mSmokeTime >= 0.05f) {
			mSmokeTime = 0.0f;
			AnimState animState;
			Animation2D anim;
			anim.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::BOSS_EFFECT_BOKO_TEX));
			anim.change_param(0);
			animState.anim = anim;
			animState.angle = GetRand(360);
			animState.position = position_ + Vector2(CHIPSIZE / 2, CHIPSIZE / 2) + Vector2(GetRand(CHIPSIZE * 6), 0.0f);
			mSmokeAnims.push_back(animState);
		}
	}
	if (mSmokeAnims.empty())return;
	for (auto & i : mSmokeAnims) {
		i.anim.update(deltaTime);

		i.scale += Time::GetInstance().deltaTime();
		i.scale = MathHelper::Clamp(i.scale, 0.0f, 1.0f);
		if (i.scale >= 1.0f)
			i.alpha -= Time::GetInstance().deltaTime();
		else
			i.alpha += 2.0f*Time::GetInstance().deltaTime();


	}
	//ÉøÇ™É[Éçà»â∫ÇæÇ¡ÇΩÇÁçÌèú
	auto a = std::remove_if(mSmokeAnims.begin(), mSmokeAnims.end(), [](AnimState state)
	{return state.alpha <= 0.0f; });
	mSmokeAnims.erase(a, mSmokeAnims.end());
}

void Smoke::onDraw() const
{
	if (mSmokeAnims.empty())return;
	for (const auto& i : mSmokeAnims) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, i.alpha));
		i.anim.draw(i.position, mAnimSize, Vector2(i.scale, i.scale), i.angle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);
	}
}

void Smoke::onCollide(Actor & other)
{
}

void Smoke::SmokeFlag(bool flag)
{
	mIsSmoke = flag;
}


