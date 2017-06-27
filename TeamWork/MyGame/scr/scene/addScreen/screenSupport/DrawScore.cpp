#include "DrawScore.h"
#include<DxLib.h>
#include"../../../graphic/Sprite.h"
#include"../../../math/MyFuncionList.h"
#include"../../../math/MathHelper.h"

DrawScore::DrawScore()
{
	numberSpriteSize_ = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).x;
}

DrawScore::~DrawScore()
{
}

int DrawScore::Draw(const Vector2& position,int score,int digit,const Vector2& numberSize)
{	
	int maxscore = MathHelper::Pow(10,digit)-1;
	score = min(score, maxscore);
	Vector2 baseSize=Vector2(Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).x*numberSize.x,
		Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).y*numberSize.y);

	Vector2 basePos = position-Vector2(DigitLength(score)*baseSize.x,0.f);
	std::vector<int> drawscore=SligeDigit(score);
	
	for (int i = 0; i < (int)drawscore.size(); i++) {
		Sprite::GetInstance().SplitDraw(SPRITE_ID::NUMBER_SPRITE,
			basePos+Vector2((baseSize.x)*(i),0.f),drawscore.at(drawscore.size()-1-i), baseSize/2,numberSize);
	}
	int result = (int)basePos.x;
	result += Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).x*(1 - (int)numberSize.x);
	return result;
}
