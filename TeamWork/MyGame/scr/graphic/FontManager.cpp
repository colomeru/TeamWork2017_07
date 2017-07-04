#include "FontManager.h"
#include "../game/WorkFolder.h"


FontManager::~FontManager()
{
	End();
	fontids_.clear();
}

void FontManager::Initialize()
{
	End();
	fontids_.clear();
	SetFontSize(16);
}

void FontManager::AddFont(FONT_ID id,const std::string & filename,const std::string& fontname,int fontSize)
{
	int i=AddFontResourceEx((WorkFolder::Name + filename).c_str(), FR_PRIVATE, NULL);
	fontids_[id] = CreateFontToHandle(fontname.c_str(), fontSize, 3, DX_FONTTYPE_ANTIALIASING_8X8, DX_CHARSET_DEFAULT);

}


int FontManager::GetFontHandle(FONT_ID id)const
{
	return fontids_.at(id);
}

void FontManager::DrawTextApplyFont(int x, int y, unsigned int color, FONT_ID id, const std::string & text,...)
{
	DrawFormatStringToHandle(x, y, color, fontids_[id], text.c_str());


}

void FontManager::End()
{
	InitFontToHandle();
}
