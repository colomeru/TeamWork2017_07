#pragma once

#include<DxLib.h>
#include<map>

enum FontName {
	LoadingFont,
	TitleFont,
	GamePlayFont,
	MoveScreenFont,
	GameOverFont,
	GameEndFont
};

class FontManager {
public:
	FontManager();
	static FontManager & GetInstance();
	int ChangeFont(FontName targetFont);
	int GetFontSize(FontName targetFont);
private:
	std::map<FontName,int> fontData_;
	std::map<FontName,int> fontSize_;
};