#include "DrawStar.h"
#include"../../../graphic/Sprite.h"
#include"../../../tween/TweenManager.h"


static int speed = 3;
DrawStar::DrawStar(int delayTime):starCount_(0), fstarCount_(0.f),isSetStar_(false)
{

}

DrawStar::~DrawStar()
{
}

void DrawStar::Update()
{
	if (!isSetStar_)return;
	for (int i = 0; i < (int)roundf(fstarCount_);i++) {
		timeCount_[i]++;
		timeCount_[i] = min(timeCount_[i], (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE) - 1)*speed);
	}
	//timeCount_++;
	//timeCount_ = min(timeCount_, (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE)-1)*speed);
	//timeCount_ %= Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE);
}

void DrawStar::Draw(const Vector2& position) const
{
	if (!isSetStar_)return;
	for (int i = 0; i < (int)roundf(fstarCount_); i++) {
		Sprite::GetInstance().SplitDraw(SPRITE_ID::SCORE_STAR_SPRITE, position+ Vector2(Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::SCORE_STAR_SPRITE).x*i,0.f), timeCount_[i]/ speed);
	}
}

void DrawStar::SetStarCount(int count) {
	starCount_ = count;
	TweenManager::GetInstance().Add(Linear, &fstarCount_, (float)count, 1.f);
	isSetStar_ = true;
	timeCount_.resize(count, 0);
}
void DrawStar::SetFullStarCount(int count) {
	starCount_ = count;
	//TweenManager::GetInstance().Add(Linear, &fstarCount_, (float)count, 1.f);
	isSetStar_ = true;
	timeCount_.resize(count, 0);
}

void DrawStar::SetFull()
{
	TweenManager::GetInstance().Cancel(&fstarCount_);
	fstarCount_ = starCount_;
	for (int i = 0; i < (int)roundf(fstarCount_); i++) {
		timeCount_[i] = (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE) - 1)*speed;
	}


}
