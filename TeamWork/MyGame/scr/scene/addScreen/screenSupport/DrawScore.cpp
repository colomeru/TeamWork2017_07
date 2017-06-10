#include "DrawScore.h"
#include<DxLib.h>
#include"../../../graphic/Sprite.h"
#include<sstream>
#include<iomanip>
#include"../../../math/MathHelper.h"

DrawScore::DrawScore()
{
	numberSpriteSize_ = Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).x;
}

DrawScore::~DrawScore()
{
}

void DrawScore::Draw(const Vector2& position,int score,int digit)
{
	//SPRITE_ID::NUMBER_SPRITE
	
	
	int drawscore = min(MathHelper::Pow(10, digit - 1), score);
	if (drawscore > 1)drawscore--;

	std::ostringstream sstr;
	sstr << std::setw(digit) << std::setfill('0') << score;

	std::string drawscoretext = sstr.str();

	for (int i = 0; i < drawscoretext.size(); i++) {
		Sprite::GetInstance().SplitDraw(SPRITE_ID::NUMBER_SPRITE,
			position+Vector2(numberSpriteSize_*i,0.f),((int)drawscoretext.at(i))-48);
	}

}
