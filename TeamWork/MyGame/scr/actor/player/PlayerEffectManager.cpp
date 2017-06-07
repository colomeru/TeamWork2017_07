#include "PlayerEffectManager.h"
#include"../../graphic/Sprite.h"
#include"../../game/ID.h"
#include"../../math/Vector2.h"
#include"../../time/Time.h"

PlayerEffectManager::PlayerEffectManager(SPRITE_ID id,int anmSpeed):anmSpeed_(anmSpeed),frameCount_(0),id_(id),isEnd_(false)
{
	maxFrame_ = Sprite::GetInstance().GetSliptFrameSize(id);
	//Sprite::GetInstance().GetCount
	//maxFrame_ = (int)roundf(splitCount.x*splitCount.y);
}

PlayerEffectManager::~PlayerEffectManager()
{
}

void PlayerEffectManager::Update()
{
	frameCount_++;
	if (frameCount_ >= maxFrame_*anmSpeed_) {
		isEnd_ = true;
	}
	frameCount_=min(frameCount_,(maxFrame_*anmSpeed_)-1);
}



void PlayerEffectManager::DrawEffect(const Vector2& position) const
{
	int drawFrame = (frameCount_ / anmSpeed_);
	Vector2 origin = Sprite::GetInstance().GetSplitPieceSize(id_)/2;
	Sprite::GetInstance().SplitDraw(id_, position,drawFrame,origin,Vector2::One);
}

bool PlayerEffectManager::GetIsEnd() const
{
	return isEnd_;
}
