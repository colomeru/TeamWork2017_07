#include "DrawStar.h"
#include"../../../graphic/Sprite.h"

static int speed = 3;
DrawStar::DrawStar():timeCount_(0), starCount_(0)
{

}

DrawStar::~DrawStar()
{
}

void DrawStar::Update()
{
	timeCount_++;
	timeCount_ = min(timeCount_, (Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE)-1)*speed);
	//timeCount_ %= Sprite::GetInstance().GetSliptFrameSize(SPRITE_ID::SCORE_STAR_SPRITE);
}

void DrawStar::Draw(const Vector2& position) const
{
	for (int i = 0; i < starCount_; i++) {
		Sprite::GetInstance().SplitDraw(SPRITE_ID::SCORE_STAR_SPRITE, position+ Vector2(Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::SCORE_STAR_SPRITE).x*i,0.f), timeCount_/ speed);
	}
}
