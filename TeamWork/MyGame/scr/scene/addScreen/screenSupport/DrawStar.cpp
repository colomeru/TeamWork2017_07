#include "DrawStar.h"
#include"../../../graphic/Sprite.h"
#include"../../../tween/TweenManager.h"
#include"../../../sound/sound.h"


static int speed = 3;
DrawStar::DrawStar(int delayTime):starCount_(0), fstarCount_(0.f),isSetStar_(false), isFullStar_(false)
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

	int size = isStarUp_.size();
	if (size > 0) {
		if ((int)roundf(fstarCount_) >= 1 && !isStarUp_[0]) {
			Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
			isStarUp_[0] = true;
		}
	}
	if (size > 1) {
		if ((int)roundf(fstarCount_) >= 2 && !isStarUp_[1]) {
			Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
			isStarUp_[1] = true;
		}
	}
	if (size > 2) {
		if ((int)roundf(fstarCount_) >= 3 && !isStarUp_[2]) {
			Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
			isStarUp_[2] = true;
		}
	}
	//timeCount_++;
	//timeCount_ = min(timeCount_, (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE)-1)*speed);
	//timeCount_ %= Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE);
}

void DrawStar::Draw(const Vector2& position) const
{
	if (!isSetStar_)return;
	int drawCount = (int)roundf(fstarCount_);
	if (isFullStar_)drawCount = starCount_;
	
	Vector2 targethalfSize = Vector2(Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::SCORE_STAR_SPRITE).x / 2, 0.f);
	for (int i = 0; i < drawCount; i++) {
		Vector2 drawPos = position;// + Vector2(Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::SCORE_STAR_SPRITE).x*i, 0.f);

		drawPos -= targethalfSize;
		drawPos -= (drawCount - 1)*targethalfSize;
		drawPos += (targethalfSize * 2)*i;
		Sprite::GetInstance().SplitDraw(SPRITE_ID::SCORE_STAR_SPRITE, drawPos, timeCount_[i]/ speed);
	}
}

void DrawStar::SetStarCount(int count) {
	starCount_ = count;
	TweenManager::GetInstance().Add(Linear, &fstarCount_, (float)count, 1.f, [=] {isFullStar_ = true; });
	isSetStar_ = true;
	timeCount_.resize(count, 0);
	isStarUp_.resize(count, false);

}
void DrawStar::SetFullStarCount(int count) {
	starCount_ = count;
	//TweenManager::GetInstance().Cancel(&fstarCount_);
	isSetStar_ = true;
	timeCount_.resize(count, 0);
	isStarUp_.resize(count, false);
}

void DrawStar::SetFull()
{
	TweenManager::GetInstance().Cancel(&fstarCount_);
	fstarCount_ = starCount_;
	for (int i = 0; i < (int)roundf(fstarCount_); i++) {
		timeCount_[i] = (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE) - 1)*speed;
	}
	for (auto& i : isStarUp_) {
		i = true;
	}
	isFullStar_ = true;

}

bool DrawStar::GetStarFull() const
{
	return isFullStar_;
}
