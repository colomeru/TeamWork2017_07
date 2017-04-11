#include "NumberTexture.h"
#include <sstream>
#include <iomanip>
#include "../ResourceLoader/ResourceLoader.h"
NumberTexture::NumberTexture(TextureID texture, int width, int height) :
	mTexture(texture),
	mWidth(width),
	mHeight(height)
{
}

void NumberTexture::draw2(const Vector2 & position, int num, int digit, Vector3 color, float scale,char fill) const
{
	std::stringstream ss;
	ss << std::setw(digit) << std::setfill(fill) << num;
	draw(position, ss.str(), color,scale);
}

void NumberTexture::draw(const Vector2 & position, int num, Vector3 color, float scale) const
{
	draw(position, std::to_string(num), color, scale);
}

void NumberTexture::draw(const Vector2 & position, const std::string & num, Vector3 color, float scale) const
{
	float widhtScale = mWidth*scale;
	float heightScale = mHeight*scale;
	for (int i = 0; i < (int)num.size(); ++i) {
		if (num[i] == ' ') continue;
		const int n = num[i] - '0';
		const Vector2 pos(position.x + i * mWidth*scale, position.y);
		DrawRectExtendGraph(pos.x, pos.y, pos.x + widhtScale, pos.y + heightScale, n * mWidth, 0.0f, mWidth, mHeight,ResourceLoader::GetInstance().getTextureID(mTexture), TRUE);
		//DrawRectGraph(pos.x, pos.y, n * mWidth, 0.0f, mWidth , mHeight, Sprite::GetInstance().GetIndex(mTexture) , TRUE, FALSE);	}
	}
}
