#include "TextDraw.h"

void TextDraw::Draw(const std::string& s)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%s\n", s.c_str());
}



void TextDraw::Draw(const Vector2& pos, const Vector3& color, const std::string& s)
{
	DrawFormatString((int)pos.x, (int)pos.y, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Vector2& pos, const std::string& s)
{
	DrawFormatString((int)pos.x, (int)pos.y, GetColor(255, 255, 255), "%s\n", s.c_str());
}
void TextDraw::Draw(const Vector2& pos, const Vector3& color, const float& size, const std::string& s)
{
	DrawExtendFormatString((int)pos.x, (int)pos.y, (double)size, (double)size, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Vector2& pos, const Vector3& color, const double& size, const std::string& s)
{
	DrawExtendFormatString((int)pos.x, (int)pos.y, size, size, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Vector2& pos, const Vector3& color, const float& sizeX, const float& sizeY, const std::string& s)
{
	DrawExtendFormatString((int)pos.x, (int)pos.y, (double)sizeX, (double)sizeY, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Vector2& pos, const Vector3& color, const double& sizeX, const double& sizeY, const std::string& s)
{
	DrawExtendFormatString((int)pos.x, (int)pos.y, sizeX, sizeY, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}



void TextDraw::Draw(const Point& pos, const Vector3& color, const std::string& s)
{
	DrawFormatString(pos.x, pos.y, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Point& pos, const std::string& s)
{
	DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "%s\n", s.c_str());
}
void TextDraw::Draw(const Point& pos, const Vector3& color, const float& size, const std::string& s)
{
	DrawExtendFormatString(pos.x, pos.y, (double)size, (double)size, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Point& pos, const Vector3& color, const double& size, const std::string& s)
{
	DrawExtendFormatString(pos.x, pos.y, size, size, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Point& pos, const Vector3& color, const float& sizeX, const float& sizeY, const std::string& s)
{
	DrawExtendFormatString(pos.x, pos.y, (double)sizeX, (double)sizeY, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
void TextDraw::Draw(const Point& pos, const Vector3& color, const double& sizeX, const double& sizeY, const std::string& s)
{
	DrawExtendFormatString(pos.x, pos.y, sizeX, sizeY, GetColor((int)color.x, (int)color.y, (int)color.z), "%s\n", s.c_str());
}
